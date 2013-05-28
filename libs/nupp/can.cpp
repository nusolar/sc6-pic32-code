#include "nupp/errorcodes.hpp"
#include <GenericTypeDefs.h>

#include "nupp/can.hpp"
#include "nupp/timer.hpp"
#include "nu/utility/bits.h"
#undef Module

using namespace nu;
using namespace can;


#if 1
CAN_BIT_CONFIG Module::default_cfg = {
	/* .phaseSeg2Tq            = */ CAN_BIT_5TQ,
	/* .phaseSeg1Tq            = */ CAN_BIT_3TQ,
	/* .propagationSegTq       = */ CAN_BIT_1TQ,
	/* .phaseSeg2TimeSelect    = */ (BOOL) AUTO_SET,
	/* .sample3Time            = */ (BOOL) THREE_TIMES,
	/* .syncJumpWidth          = */ CAN_BIT_1TQ
};
#else
CAN_BIT_CONFIG Module::default_cfg = {
	/* .phaseSeg2Tq            = */ CAN_BIT_3TQ,
	/* .phaseSeg1Tq            = */ CAN_BIT_3TQ,
	/* .propagationSegTq       = */ CAN_BIT_3TQ,
	/* .phaseSeg2TimeSelect    = */ (BOOL) AUTO_SET,
	/* .sample3Time            = */ (BOOL) THREE_TIMES,
	/* .syncJumpWidth          = */ CAN_BIT_2TQ
};
#endif

Module::Module(CAN_MODULE _mod, uint32_t bus_speed,
			   CAN_BIT_CONFIG *timings,
			   CAN_MODULE_EVENT interrupts,
			   INT_PRIORITY int_priority,
			   CAN_MODULE_FEATURES features): mod(_mod)
{
	CANEnableModule(mod, TRUE);

	int32_t err_num = config_mode();
	if (err_num < 0) {
		normal_mode();
//		return err_num; // WARNING
	}

	CANSetSpeed(mod, timings, NU_HZ, bus_speed);
	CANAssignMemoryBuffer(mod, buf, sizeof(buf));

	if (interrupts) {
		INT_VECTOR int_vec;
        INT_SOURCE int_src;

        CANEnableModuleEvent(CAN1, interrupts, TRUE);

        switch (mod) {
			case CAN1:
				int_vec = INT_CAN_1_VECTOR;
				int_src = INT_CAN1;
				break;
			case CAN2:
				int_vec = INT_CAN_2_VECTOR;
				int_src = INT_CAN2;
				break;
			case CAN_NUMBER_OF_MODULES:
			default:
				return /*-EINVAL*/; // WARNING
        }

        INTSetVectorPriority(int_vec, int_priority);
        INTSetVectorSubPriority(int_vec, INT_SUB_PRIORITY_LEVEL_0);
        INTEnable(int_src, INT_ENABLED);
	}
	change_features(features, TRUE);
	if ((err_num = normal_mode()) < 0) return /*err_num*/; // WARNING

	for (size_t i=0; i<32; i++) {
		channel(i) = Channel(*this, (CAN_CHANNEL)i);
	}
}


ALWAYSINLINE
int32_t Module::switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms) {
	uint32_t start = timer::us();
	CANSetOperatingMode(mod, op_mode);
	while (timer::us() - start < timeout_ms*1000)
		if (CANGetOperatingMode(mod) == op_mode)
			return 0;
	return -error::ETIMEOUT;
}


ALWAYSINLINE
int32_t Module::change_features(CAN_MODULE_FEATURES features, BOOL en) {
	int32_t err_num = config_mode();
	if (err_num < 0) {
		normal_mode();
		return err_num;
	}
	CANEnableFeature(mod, features, en);
	return 0;
}


Channel::Channel(Module &_mod, CAN_CHANNEL _chn, uint32_t _msg_size, CAN_CHANNEL_EVENT _inter):
	config(UNCONFIGURED), mod(_mod), chn(_chn), msg_size(_msg_size), interrupts(_inter), data_mode(CAN_RX_FULL_RECEIVE), priority(CAN_LOWEST_PRIORITY), rtr_en(CAN_TX_RTR_DISABLED) {}


/**
 * Configure CAN Channel for RX.
 * @param _data_mode Whether we should receive entire packet, or just data.
 * @return Error code
 * @todo Allow Interrupts!
 */
int32_t Channel::setup_rx(CAN_RX_DATA_MODE _data_mode) {
	data_mode = _data_mode;
	
	int32_t err = mod.config_mode();
	if (err < 0) {
		mod.normal_mode();
		return err;
	}
	CANConfigureChannelForRx(mod, chn, msg_size, data_mode);
	CANEnableChannelEvent(mod, chn, interrupts, TRUE);
	this->config = RX;
	if ((err = mod.normal_mode()) < 0) return err;
	return 0;
}


/**
 * Configure CAN Channel for TX.
 * @param _priority The priority of this TX Channel.
 * @param _rtr_en Whether the TX will send RTR packets.
 * @return Error code
 * @todo Allow interrupts!
 */
int32_t Channel::setup_tx(CAN_TXCHANNEL_PRIORITY _priority, CAN_TX_RTR _rtr_en) {
	priority = _priority;
	rtr_en = _rtr_en;
	
	int32_t err = mod.config_mode();
	if (err < 0) {
		mod.normal_mode();
		return err;
	}
	CANConfigureChannelForTx(mod, chn, msg_size, rtr_en, priority);
	CANEnableChannelEvent(mod, chn, interrupts, TRUE);
	this->config = TX;
	if ((err = mod.normal_mode()) < 0) return err;
	return 0;
}


/**
 * Read a message from the CAN Channel.
 * @param dest Memory for CAN message to be copied to.
 * @param id ID of CAN message.
 * @return Number of bytes read copied.
 */
size_t Channel::rx(void *dest, uint32_t &id) {
	if (config != RX) return -error::EINVAL;

	CANRxMessageBuffer *buffer = CANGetRxMessage(mod, chn);
	if (buffer == NULL) return -error::ENODATA;

	id = (buffer->msgEID.IDE == STANDARD_ID)?
		buffer->msgSID.SID: buffer->msgEID.EID|buffer->msgSID.SID;

	size_t len = buffer->msgEID.DLC;
	memcpy(dest, buffer->data, len);
	CANUpdateChannel(mod, chn);

	return len;
}


/**
 * Transmit a message over the CAN Channel.
 * @param data Pointer to the message data to be copied.
 * @param num_bytes Number of bytes to copy.
 * @param std_id The SID portion of the ID. Ranges from [0x000, 0x7FF].
 * @param ext_id The EID continutation of the ID. Range is [0x0, 0x3FFFF].
 * @param type Whether message is SID or EID. If SID, then ext_id should be 0.
 * @return Error code.
 */
int32_t Channel::tx(const void *data, size_t num_bytes, uint16_t std_id, uint32_t ext_id, id_type type) {
	if (config != TX) return -error::EINVAL; // WARNING error code for wrong config?
	if (num_bytes > 8) return -error::EINVAL; // Maximum of 8 data bytes in CAN frame

	int32_t err = mod.normal_mode();
	if (err < 0) return err;
	
	CANTxMessageBuffer *msg = CANGetTxMessageBuffer(mod, chn);
	if (msg == NULL) return -error::ENULPTR;

	memset(msg, 0, sizeof(CANTxMessageBuffer));
	msg->msgEID.IDE = (EXTENDED_ID == type); // EID is indicated by IDTypeExtended = 1
	msg->msgSID.SID = BITFIELD_CAST(std_id, 11); // 11 bits
	msg->msgEID.DLC = BITFIELD_CAST(num_bytes, 4); // 4 bits
	msg->msgEID.RTR = BITFIELD_CAST(!rtr_en, 1); // 1 bit; 1 = remote transmission rqst enabled
	if (EXTENDED_ID == type)
		msg->msgEID.EID = BITFIELD_CAST(ext_id, 18); // 18 bits

	if (num_bytes) memcpy(msg->data, (const char *)data, num_bytes);

	CANUpdateChannel(mod, chn);
	CANFlushTxChannel(mod, chn);
	return 0;
}


int32_t Channel::add_filter(CAN_FILTER filter, CAN_ID_TYPE _id_type, uint32_t id,
							CAN_FILTER_MASK mask, CAN_FILTER_MASK_TYPE mide, uint32_t mask_bits) {
	if (config != RX) return -error::EINVAL; // WARNING should fail if not RX?

	int32_t err = mod.config_mode();
	if (err < 0) {
		mod.normal_mode();
		return err;
	}

	CANConfigureFilter(mod, filter, id, _id_type);
	CANConfigureFilterMask(mod, mask, mask_bits, _id_type, mide);
	CANLinkFilterToChannel(mod, filter, mask, chn);
	CANEnableFilter(mod, filter, TRUE);

	if ((err = mod.normal_mode()) < 0) return err;
	return 0;
}



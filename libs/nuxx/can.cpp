#include "nuxx/peripheral/can.hpp"
#include "nuxx/timer.hpp"
#include "nuxx/errorcodes.hpp"
#include "nu/utility/bits.h"

#include <GenericTypeDefs.h>

#ifdef Module
#undef Module
#endif

using namespace nu;
using namespace Can;


#if 1
CAN_BIT_CONFIG Module::default_cfg = {
	/* .phaseSeg2Tq            = */ CAN_BIT_5TQ,
	/* .phaseSeg1Tq            = */ CAN_BIT_3TQ,
	/* .propagationSegTq       = */ CAN_BIT_1TQ,
	/* .phaseSeg2TimeSelect    = */ (BOOL) Module::AUTO_SET,
	/* .sample3Time            = */ (BOOL) Module::THREE_TIMES,
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

nu::Can::Module::Module(CAN_MODULE _mod, uint32_t _bus_speed):
	mod(_mod),
	bus_speed(_bus_speed)
{
	for (size_t i=0; i<32; i++)
	{
		this->channel(i) = Channel(*this, (CAN_CHANNEL)i);
	}
}


int32_t nu::Can::Module::setup(CAN_BIT_CONFIG* timings,
					   CAN_MODULE_EVENT interrupts,
					   INT_PRIORITY int_priority,
					   CAN_MODULE_FEATURES features)
{
	CANEnableModule(this->mod, TRUE);

	int32_t err_num = this->config_mode();
	if (err_num < 0) {
		this->normal_mode();
		return err_num;
	}

	CANSetSpeed(this->mod, timings, NU_HZ, this->bus_speed);
	CANAssignMemoryBuffer(this->mod, this->buf, sizeof(this->buf));

	if (interrupts) {
		INT_VECTOR int_vec;
        INT_SOURCE int_src;

        CANEnableModuleEvent(CAN1, interrupts, TRUE);

        switch (this->mod) {
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
				return -EINVAL;
        }

        INTSetVectorPriority(int_vec, int_priority);
        INTSetVectorSubPriority(int_vec, INT_SUB_PRIORITY_LEVEL_0);
        INTEnable(int_src, INT_ENABLED);
	}
	change_features(features, TRUE);
	if ((err_num = this->normal_mode()) < 0) return err_num;
	return 0;
}


ALWAYSINLINE
int32_t nu::Can::Module::switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms) {
	if (CANGetOperatingMode(mod) == op_mode)
		return 0;
	
	Timer changing_mode_timer = Timer(timeout_ms, Timer::ms, true);
	CANSetOperatingMode(mod, op_mode);
	while (!changing_mode_timer.has_expired())
		if (CANGetOperatingMode(mod) == op_mode)
			return 0;
	return -error::ETIMEOUT;
}


ALWAYSINLINE
int32_t nu::Can::Module::change_features(CAN_MODULE_FEATURES features, BOOL en) {
	int32_t err_num = config_mode();
	if (err_num < 0) {
		normal_mode();
		return err_num;
	}
	CANEnableFeature(this->mod, features, en);
	return 0;
}


int32_t nu::Can::Module::setup_mask(CAN_FILTER_MASK mask, uint32_t mask_bits, CAN_ID_TYPE _id_type, CAN_FILTER_MASK_TYPE mide)
{
	int32_t _err = this->config_mode();
	if (_err != 0) {
		this->normal_mode();
		return _err;
	}

	CANConfigureFilterMask(this->mod, mask, mask_bits, _id_type, mide);
	return 0;
}


int32_t nu::Can::Module::setup_filter(CAN_FILTER filter, uint32_t id, CAN_ID_TYPE _id_type)
{
	int32_t _err = this->config_mode();
	if (_err != 0) {
		this->normal_mode();
		return _err;
	}

	CANConfigureFilter(this->mod, filter, id, _id_type);
	return 0;
}


/**
 * Configure CAN Channel for RX.
 * @param _data_mode Whether we should receive entire packet, or just data.
 * @return Error code
 * @todo Allow Interrupts!
 */
int32_t nu::Can::Channel::setup_rx(CAN_RX_DATA_MODE _data_mode, uint32_t num_messages)
{
	int32_t err = this->module.config_mode();
	if (err < 0) {
		this->module.normal_mode();
		return err;
	}
	CANConfigureChannelForRx(this->module.mod, this->chn, num_messages, _data_mode);
	CANEnableChannelEvent(this->module.mod, this->chn, this->interrupts, TRUE);
	this->config = RX;
	return 0;
}


/**
 * Configure CAN Channel for TX.
 * @param _priority The priority of this TX Channel.
 * @param _rtr_en Whether the TX will send RTR packets.
 * @return Error code
 * @todo Allow interrupts!
 */
int32_t nu::Can::Channel::setup_tx(CAN_TXCHANNEL_PRIORITY _priority, CAN_TX_RTR _rtr_en, uint32_t num_messages)
{
	this->rtr_en = _rtr_en;

	int32_t err = this->module.config_mode();
	if (err < 0) {
		this->module.normal_mode();
		return err;
	}
	CANConfigureChannelForTx(this->module.mod, this->chn, num_messages, this->rtr_en, _priority);
	CANEnableChannelEvent(this->module.mod, this->chn, this->interrupts, TRUE);
	this->config = TX;
	return 0;
}


/**
 * Read a message from the CAN Channel.
 * @param dest Memory for CAN message to be copied to.
 * @param (out) the ID of CAN message.
 * @return Number of bytes read copied.
 */
int32_t nu::Can::Channel::rx(void *dest, uint32_t &id)
{
	if (this->config != RX) return -error::EINVAL;
	int32_t err = this->module.normal_mode();
	if (err != 0) return err;

	CANRxMessageBuffer *buffer = CANGetRxMessage(this->module.mod, this->chn);
	if (buffer == NULL) return -error::ENODATA;

	id = (buffer->msgEID.IDE == STANDARD_ID)?
		buffer->msgSID.SID: buffer->msgEID.EID|buffer->msgSID.SID;

	size_t len = buffer->msgEID.DLC;
	memcpy(dest, buffer->data, len);
	CANUpdateChannel(this->module.mod, this->chn);

	return len;
}

int32_t nu::Can::Channel::rx(Packet &p, uint32_t &id)
{
	return rx((void *)p.frame().bytes, id);
}


/**
 * Transmit a message over the CAN Channel. Fails if all CAN Tx Buffers are full.
 *
 * @param data Pointer to the message data to be copied.
 * @param num_bytes Number of bytes to copy.
 * @param std_id The SID portion of the ID. Ranges from [0x000, 0x7FF].
 * @param ext_id The EID continutation of the ID. Range is [0x0, 0x3FFFF].
 * @param type Whether message is SID or EID. If SID, then ext_id should be 0.
 * @return Error code.
 */
int32_t nu::Can::Channel::tx(const void *data, size_t num_bytes, uint16_t std_id, uint32_t ext_id, id_type type)
{
	if (this->config != TX) return -error::EINVAL; // WARNING error code for wrong config?
	if (num_bytes > 8) return -error::EINVAL; // Maximum of 8 data bytes in CAN frame
	int32_t err = this->module.normal_mode();
	if (err != 0) return err;

	CANTxMessageBuffer *msg = CANGetTxMessageBuffer(this->module.mod, this->chn);
	if (msg == NULL)
		return -error::ENULPTR; // All CAN Tx Buffers are full.

	memset(msg, 0, sizeof(CANTxMessageBuffer));
	msg->msgEID.IDE = (EXTENDED_ID == type); // EID is indicated by IDTypeExtended = 1
	msg->msgSID.SID = BITFIELD_CAST(std_id, 11); // 11 bits
	msg->msgEID.DLC = BITFIELD_CAST(num_bytes, 4); // 4 bits
	msg->msgEID.RTR = BITFIELD_CAST(!this->rtr_en, 1); // 1 bit; 1 = remote transmission rqst enabled
	if (EXTENDED_ID == type)
		msg->msgEID.EID = BITFIELD_CAST(ext_id, 18); // 18 bits

	if (num_bytes) memcpy(msg->data, (const char *)data, num_bytes);

	CANUpdateChannel(this->module.mod, this->chn);
	CANFlushTxChannel(this->module.mod, this->chn);
	return 0;
}


int32_t nu::Can::Channel::tx(const Packet &p)
{
	return tx((const void *)p.frame().bytes, (size_t)8, (uint16_t)p.id());
}


int32_t nu::Can::Channel::link_filter(CAN_FILTER filter, CAN_FILTER_MASK mask)
{
	if (this->config != RX) return -error::EINVAL; // WARNING should fail if not RX?
	int32_t err = this->module.config_mode();
	if (err < 0) {
		this->module.normal_mode();
		return err;
	}

	CANLinkFilterToChannel(this->module.mod, filter, mask, this->chn);
	CANEnableFilter(this->module.mod, filter, TRUE);
	return 0;
}



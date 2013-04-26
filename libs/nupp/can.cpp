#include "nupp/errorcodes.hpp"
#include <GenericTypeDefs.h>

#include "nupp/can.hpp"
#include "nupp/timer.hpp"

using namespace nu;
using namespace can;

CAN_BIT_CONFIG Module::default_cfg = {
	/* .phaseSeg2Tq            = */ CAN_BIT_5TQ,
	/* .phaseSeg1Tq            = */ CAN_BIT_3TQ,
	/* .propagationSegTq       = */ CAN_BIT_1TQ,
	/* .phaseSeg2TimeSelect    = */ (BOOL) AUTO_SET,
	/* .sample3Time            = */ (BOOL) THREE_TIMES,
	/* .syncJumpWidth          = */ CAN_BIT_1TQ
};

int32_t Module::setup(uint32_t bus_speed, CAN_BIT_CONFIG *timings, CAN_MODULE_EVENT interrupts, INT_PRIORITY int_priority, CAN_MODULE_FEATURES features) {
	CANEnableModule(mod, TRUE);

	int32_t err_num = config_mode();
	if (err_num < 0) {
		normal_mode();
		return err_num;
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
				return -EINVAL;
        }

        INTSetVectorPriority(int_vec, int_priority);
        INTSetVectorSubPriority(int_vec, INT_SUB_PRIORITY_LEVEL_0);
        INTEnable(int_src, INT_ENABLED);
	}
	change_features(features, TRUE);
	if ((err_num = normal_mode()) < 0) return err_num;
	
	// TODO: Better collection
	if(rx.chn != CAN_ALL_CHANNELS) rx.setup();
	if(tx.chn != CAN_ALL_CHANNELS) tx.setup();
	if(ex.chn != CAN_ALL_CHANNELS) ex.setup();
	return 0;
}


ALWAYSINLINE
int32_t Module::switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms) {
	uint32_t start = timer_us();
	CANSetOperatingMode(mod, op_mode);
	while (timer_us() - start < timeout_ms*1000)
		if (CANGetOperatingMode(mod) == op_mode)
			return 0;
	return -ETIMEOUT;
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


Channel::Channel(Module &_mod, CAN_CHANNEL _chn, uint32_t _size, CAN_CHANNEL_EVENT _inter):
	mod(_mod), chn(_chn), msg_size(_size), interrupts(_inter) {}


int32_t RxChannel::setup() {
	int32_t err = mod.config_mode();
	if (err < 0) {
		mod.normal_mode();
		return err;
	}
	
	CANConfigureChannelForRx(mod, chn, msg_size, data_only);
	CANConfigureFilter(mod, CAN_FILTER0, 0, CAN_SID);
	CANConfigureFilterMask(mod, CAN_FILTER_MASK0, 0, CAN_SID, CAN_FILTER_MASK_ANY_TYPE);
	CANLinkFilterToChannel(mod, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
	CANEnableFilter(mod, CAN_FILTER0, TRUE);
	CANEnableChannelEvent(mod, chn, interrupts, TRUE);
	
	if ((err = mod.normal_mode()) < 0) return err;
	return 0;
}


int32_t TxChannel::setup() {
	int32_t err = mod.config_mode();
	if (err < 0) {
		mod.normal_mode();
		return err;
	}
	
	CANConfigureChannelForTx(mod, chn, msg_size, rtr_en, priority);
	CANEnableChannelEvent(mod, chn, interrupts, TRUE);
	
	if ((err = mod.normal_mode()) < 0) return err;
	return 0;
}


size_t RxChannel::rx(void *dest, uint32_t &id) {
	CANRxMessageBuffer *buffer = CANGetRxMessage(mod, chn);
	if (buffer == NULL) return -ENODATA;
	
	id = (buffer->msgEID.IDE == STANDARD_ID)?
	buffer->msgSID.SID: buffer->msgEID.EID|buffer->msgSID.SID;
	
	size_t len = buffer->msgEID.DLC;
	memcpy(dest, buffer->data, len);
	CANUpdateChannel(mod, chn);
	
	return len;
}


int32_t TxChannel::tx(const void *data, size_t num_bytes, uint16_t std_id, uint32_t ext_id, id_type type) {
	CANTxMessageBuffer *msg = CANGetTxMessageBuffer(mod, chn);
	if (num_bytes > 8) return -EINVAL; // Maximum of 8 data bytes in CAN frame
	
	int32_t err = mod.normal_mode();
	if (err < 0) return err;
	
	memset(msg, 0, sizeof(CANTxMessageBuffer));
	msg->msgEID.IDE = (EXTENDED_ID == type); // EID is indicated by IDTypeExtended = 1
	msg->msgSID.SID = BITFIELD_CAST(std_id, 11); // 11 bits
	msg->msgEID.DLC = BITFIELD_CAST(num_bytes, 4); // 4 bits
	msg->msgEID.RTR = BITFIELD_CAST(rtr_en, 1); // 1 bit; 1 = remote transmission rqst enabled
	if (EXTENDED_ID == type)
		msg->msgEID.EID = BITFIELD_CAST(ext_id, 18); // 18 bits
	
	if (num_bytes) memcpy(msg->data, (const char *)data, num_bytes);
	
	CANUpdateChannel(mod, chn);
	CANFlushTxChannel(mod, chn);
	return 0;
}

int32_t RxChannel::add_filter(CAN_FILTER filter, CAN_ID_TYPE f_type, uint32_t id,
							  CAN_FILTER_MASK mask, CAN_FILTER_MASK_TYPE mide, uint32_t mask_bits) {
	int32_t err = mod.config_mode();
	if (err < 0) {
		mod.normal_mode();
		return err;
	}
	
	CANConfigureFilter(mod, filter, id, f_type);
	CANConfigureFilterMask(mod, mask, mask_bits, f_type, mide);
	CANLinkFilterToChannel(mod, filter, mask, chn);
	CANEnableFilter(mod, filter, TRUE);
	
	if ((err = mod.normal_mode()) < 0) return err;
	return 0;
}



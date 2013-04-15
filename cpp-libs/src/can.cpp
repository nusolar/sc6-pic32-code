//
//  can.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "errorcodes.h"
#include <GenericTypeDefs.h>

#include "can.h"
#include "timer.h"

using namespace nu;
using namespace can;

#define DEFAULT_BUS_SPEED_HZ    1E6
CAN_BIT_CONFIG CAN::default_cfg = {
	/* .phaseSeg2Tq            = */ CAN_BIT_5TQ,
	/* .phaseSeg1Tq            = */ CAN_BIT_3TQ,
	/* .propagationSegTq       = */ CAN_BIT_1TQ,
	/* .phaseSeg2TimeSelect    = */ (BOOL) AUTO_SET,
	/* .sample3Time            = */ (BOOL) THREE_TIMES,
	/* .syncJumpWidth          = */ CAN_BIT_1TQ
};

int32_t CAN::setup(uint32_t bus_speed = DEFAULT_BUS_SPEED_HZ, CAN_BIT_CONFIG *timings = &default_cfg, CAN_MODULE_EVENT interrupts = (CAN_MODULE_EVENT)0, INT_PRIORITY int_priority = INT_PRIORITY_DISABLED, CAN_MODULE_FEATURES features = (CAN_MODULE_FEATURES)0) {

	CANEnableModule(mod, TRUE);

	int32_t err = config_mode();
	if (err < 0) {
		normal_mode();
		return err;
	}

	CANSetSpeed(mod, timings, HZ, bus_speed);
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
	if ((err = normal_mode()) < 0) return err;
	return 0;
}

int32_t CAN::switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms) {
	uint32_t start = timer_us();
	CANSetOperatingMode(mod, op_mode);
	while (timer_us() - start < timeout_ms*1000)
		if (CANGetOperatingMode(mod) == op_mode)
			return 0;
	return -ETIMEOUT;
}

int32_t CAN::change_features(CAN_MODULE_FEATURES features, BOOL en) {
	int32_t err = config_mode();
	if (err < 0) {
		normal_mode();
		return err;
	}
	CANEnableFeature(mod, features, en);
	return 0;
}

size_t CAN::rx(void *dest, uint32_t &id) {
	CANRxMessageBuffer *buffer = CANGetRxMessage(mod, chn);
	if (buffer == NULL) return -ENODATA;

	id = (buffer->msgEID.IDE == STANDARD_ID)?
		buffer->msgSID.SID: buffer->msgEID.EID|buffer->msgSID.SID;

	size_t len = buffer->msgEID.DLC;
	memcpy(dest, buffer->data, len);
	CANUpdateChannel(mod, chn);

	return len;
}

int32_t CAN::tx(const void *data, size_t n, uint32_t rtr_en) {
	CANTxMessageBuffer *msg = CANGetTxMessageBuffer(mod, chn);
	if (n > 8) return -EINVAL;

	int32_t err = normal_mode();
	if (err < 0) return err;

	memset(msg, 0, sizeof(CANTxMessageBuffer));
	msg->msgEID.IDE = (EXTENDED_ID == type); // EID is indicated by IDTypeExtended = 1
	msg->msgSID.SID = BITFIELD_CAST(std_id, 11); // 11 bits
	msg->msgEID.DLC = BITFIELD_CAST(n, 4); // 4 bits
	msg->msgEID.RTR = BITFIELD_CAST(rtr_en, 1); // 1 bit; 1 = remote transmission rqst enabled
	if (EXTENDED_ID == type)
		msg->msgEID.EID = BITFIELD_CAST(ext_id, 18); // 18 bits

	if (n) memcpy(msg->data, (const char *)data, n);

	CANUpdateChannel(mod, chn);
	CANFlushTxChannel(mod, chn);
	return 0;
}

int32_t CAN::add_rx(CAN_CHANNEL chn, uint32_t msg_size, CAN_RX_DATA_MODE data_only,
	CAN_CHANNEL_EVENT interrupts) {
	int32_t err = config_mode();
	if (err < 0) {
		normal_mode();
		return err;
	}

	CANConfigureChannelForRx(mod, chn, msg_size, data_only);
	CANConfigureFilter(mod, CAN_FILTER0, 0, CAN_SID);
	CANConfigureFilterMask(mod, CAN_FILTER_MASK0, 0, CAN_SID, CAN_FILTER_MASK_ANY_TYPE);
	CANLinkFilterToChannel(mod, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
	CANEnableFilter(mod, CAN_FILTER0, TRUE);
	CANEnableChannelEvent(mod, chn, interrupts, TRUE);

	if ((err = normal_mode()) < 0) return err;
	return 0;
}

int32_t CAN::add_tx(CAN_CHANNEL chn, uint32_t msg_size, CAN_TX_RTR rtr_en,
	CAN_TXCHANNEL_PRIORITY priority, CAN_CHANNEL_EVENT interrupts) {
	int32_t err = config_mode();
	if (err < 0) {
		normal_mode();
		return err;
	}

	CANConfigureChannelForTx(mod, chn, msg_size, rtr_en, priority);
	CANEnableChannelEvent(mod, chn, interrupts, TRUE);

	if ((err = normal_mode()) < 0) return err;
	return 0;
}

int32_t CAN::add_filter(CAN_CHANNEL chn, CAN_FILTER filter, CAN_ID_TYPE f_type, uint32_t id,
		CAN_FILTER_MASK mask, CAN_FILTER_MASK_TYPE mide, uint32_t mask_bits) {
	int32_t err = config_mode();
	if (err < 0) {
		normal_mode();
		return err;
	}

	CANConfigureFilter(mod, filter, id, f_type);
	CANConfigureFilterMask(mod, mask, mask_bits, f_type, mide);
	CANLinkFilterToChannel(mod, filter, mask, chn);
	CANEnableFilter(mod, filter, TRUE);

	if ((err = normal_mode()) < 0) return err;
	return 0;
}




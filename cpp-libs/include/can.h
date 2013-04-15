//
//  can.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__can__
#define __nusolar_lib__can__

#include "compiler.h"
#include <stdint.h>

#include <peripheral/CAN.h>
#include <peripheral/int.h>

namespace nu {
	namespace can {
		struct CAN {
			enum id_type {
				STANDARD_ID,
				EXTENDED_ID
			};
			enum phase_seg2_time_select {
				MAN_SET,
				AUTO_SET
			};
			enum sample_times {
				ONE_TIME,
				THREE_TIMES
			};
			static CAN_BIT_CONFIG default_cfg;

			CAN_MODULE mod;
			CAN_CHANNEL chn;
			id_type type;
			uint16_t std_id;
			uint16_t ext_id;
			char buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES];

			INLINE CAN(CAN_MODULE _mod, CAN_CHANNEL _chn = CAN_CHANNEL0, id_type _type = STANDARD_ID, uint16_t _std = 0, uint16_t _ext = 0): mod(_mod), chn(_chn), type(_type), std_id(_std), ext_id(_ext) {}

			int32_t setup(uint32_t bus_speed, CAN_BIT_CONFIG *timings, CAN_MODULE_EVENT interrupts, INT_PRIORITY int_priority, CAN_MODULE_FEATURES features);

			size_t rx(void *dest, uint32_t &id);
			int32_t tx(const void *data, size_t n, uint32_t rtr_en);
			int32_t add_rx(CAN_CHANNEL chn, uint32_t msg_size, CAN_RX_DATA_MODE data_only, CAN_CHANNEL_EVENT interrupts);
			int32_t add_tx(CAN_CHANNEL chn, uint32_t msg_size, CAN_TX_RTR rtr_en, CAN_TXCHANNEL_PRIORITY priority, CAN_CHANNEL_EVENT interrupts);
			int32_t add_filter(CAN_CHANNEL chn, CAN_FILTER filter, CAN_ID_TYPE f_type, uint32_t id, CAN_FILTER_MASK mask, CAN_FILTER_MASK_TYPE mide, uint32_t mask_bits);

		private:
			int32_t ALWAYSINLINE switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms);
			int32_t ALWAYSINLINE normal_mode() {return switch_mode(CAN_NORMAL_OPERATION, 1);}
			int32_t ALWAYSINLINE config_mode() {return switch_mode(CAN_CONFIGURATION, 1);}
			int32_t ALWAYSINLINE change_features(CAN_MODULE_FEATURES features, BOOL en);
		};

		namespace frame {
			// Data Types:
#define Empty()
#define Module(u1, f1)\
	uint32_t u1;\
	float f1;
#define Double(d1)\
	double d1;
#define Float2(f1, f2)\
	float f1, f2;
#define UInt64(u1)\
	uint64_t u1;
#define UInt2(u1, u2)\
	uint32_t u1, u2;
#define Trip(s1, u1)\
	int32_t s1;\
	uint32_t u1;
#define Error()\
	char msg[8];
#define Status(str, u1)\
	char str[4];\
	uint32_t u1;

			// Specialty Data Types:
#define motor_Status()\
	uint16_t limitFlags;\
	uint16_t errorFlags;\
	uint16_t activeMotor;\
	uint16_t reserved __attribute__ ((__packed__));
#define sw_Lights()\
	unsigned    left            :1;\
	unsigned    right           :1;\
	unsigned    radio           :1;\
	unsigned    yes             :1;\
	unsigned    hazard          :1;\
	unsigned    cruise_en       :1;\
	unsigned    cruise_up       :1;\
	unsigned    maybe           :1;\
	unsigned    no              :1;\
	unsigned    horn            :1;\
	unsigned    cruise_mode     :1;\
	unsigned    cruise_down     :1;\
	unsigned    reserved        :20;
#define sw_Buttons()\
	unsigned    left            :1;\
	unsigned    right           :1;\
	unsigned    yes             :1;\
	unsigned    no              :1;\
	unsigned    maybe           :1;\
	unsigned    hazard          :1;\
	unsigned    horn            :1;\
	unsigned    cruise_en       :1;\
	unsigned    cruise_mode     :1;\
	unsigned    cruise_up       :1;\
	unsigned    cruise_down     :1;\
	unsigned    reserved        :21;

			// Declaration:
#define List(x) namespace x
#define Xbase int base
#define X(name, type, ...) ; struct PACKED name{type(__VA_ARGS__)}; int zzz__##name
#define end ;
#include "can.def.h"
#undef end
#undef X
#undef Xbase
#undef List
		}

		namespace addr {
			// Declaration:
#define List(x) enum class x: int
#define Xbase base
#define X(name, type, ...) , name
#define end
#include "can.def.h"
#undef end
#undef X
#undef Xbase
#undef List
		}
	}
}

#endif /* defined(__nusolar_lib__can__) */

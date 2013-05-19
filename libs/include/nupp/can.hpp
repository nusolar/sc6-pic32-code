#ifndef NUPP_CAN_HPP
#define NUPP_CAN_HPP 1

#include "nu/compiler.h"
#include <cstdint>
#include <new>

#include <plib.h>
#include "nupp/can_def.hpp"

namespace nu {
	namespace can {
		struct Module;

		/**
		 * Base type of RxChannel and TxChannel. Declared before Module so that
		 * a Module can contain Channels, but RxChannel and TxChannel
		 * can have a &reference to their Module.
		 */
		struct Channel {
			enum id_type {
				STANDARD_ID,
				EXTENDED_ID
			};
			Module &mod; // reference to parent, after setup(Module&)
			CAN_CHANNEL chn;
			uint32_t msg_size;
			CAN_CHANNEL_EVENT interrupts;

			Channel(Module &_mod, CAN_CHANNEL _chn = CAN_ALL_CHANNELS, uint32_t _size = 32,
					CAN_CHANNEL_EVENT _inter = (CAN_CHANNEL_EVENT)0);
			Channel(const Channel &a): mod(a.mod), chn(a.chn), msg_size(a.msg_size), interrupts(a.interrupts){}
			virtual ~Channel() {};
			Channel &operator= (const Channel &a) {
				if (this != &a){
					this->Channel::~Channel();
					new (this) Channel(a); // Could use memcpy() instead
				} return *this;
			}
		};
		struct RxChannel: public Channel {
			CAN_RX_DATA_MODE data_only;

			RxChannel(Channel _chn, CAN_RX_DATA_MODE _mode = CAN_RX_FULL_RECEIVE):
				Channel(_chn), data_only(_mode) {}
			int32_t setup();
			size_t rx(void *dest, uint32_t &id);
			int32_t add_filter(CAN_FILTER filter, CAN_ID_TYPE f_type, uint32_t id,
							   CAN_FILTER_MASK mask, CAN_FILTER_MASK_TYPE mide,
							   uint32_t mask_bits);
		};
		struct TxChannel: public Channel {
			CAN_TXCHANNEL_PRIORITY priority;
			CAN_TX_RTR rtr_en;

			TxChannel(Channel _chn, CAN_TXCHANNEL_PRIORITY _priority, CAN_TX_RTR _rtr = CAN_TX_RTR_DISABLED): Channel(_chn), priority(_priority), rtr_en(_rtr){}
			int32_t setup();
			int32_t tx(const void *data, size_t num_bytes, uint16_t std_id,
					   uint32_t ext_id = 0, id_type type = STANDARD_ID);
		};


		/**
		 * CAN Module. Has can::Channel fields.
		 * @todo: Allow all 32 CAN channels, setup Channels.
		 */
		struct Module {
			enum phase_seg2_time_select {
				MAN_SET,
				AUTO_SET
			};
			enum sample_times {
				ONE_TIME,
				THREE_TIMES
			};
			static const uint32_t DEFAULT_BUS_SPEED_HZ = 1E6;
			static CAN_BIT_CONFIG default_cfg;

			CAN_MODULE mod;
			char buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES];

			Module(CAN_MODULE _mod = CAN1, uint32_t bus_speed = DEFAULT_BUS_SPEED_HZ,
				   CAN_BIT_CONFIG *timings = &default_cfg,
				   CAN_MODULE_EVENT interrupts = (CAN_MODULE_EVENT)0,
				   INT_PRIORITY int_priority = INT_PRIORITY_DISABLED,
				   CAN_MODULE_FEATURES features = (CAN_MODULE_FEATURES)0);
			operator CAN_MODULE() const {return mod;}


			RxChannel &in() {return rx;}
			TxChannel &out() {return tx;}
			TxChannel &err() {return ex;}

			/** FOR USE BY RxChannel & TxChannel ONLY */
			int32_t ALWAYSINLINE normal_mode() {return switch_mode(CAN_NORMAL_OPERATION, 1);}
			int32_t ALWAYSINLINE config_mode() {return switch_mode(CAN_CONFIGURATION, 1);}
		private:
			RxChannel rx;
			TxChannel tx, ex;
			int32_t switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms);
			int32_t change_features(CAN_MODULE_FEATURES features, BOOL en);
		};
	}
}

#endif

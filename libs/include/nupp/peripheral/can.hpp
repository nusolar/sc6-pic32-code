#ifndef NUPP_CAN_HPP
#define NUPP_CAN_HPP 1

#include "nu/compiler.h"
#include <cstdint>
#include <new>
extern "C" {
#include <plib.h>
}

#include "nupp/peripheral/can_def.hpp"

namespace nu {
	namespace can {
		struct Module;

		/**
		 * A TX or RX Channel. Holds a &reference to its Module. Must be setup
		 * as TX or RX before any use.
		 */
		struct Channel {
			enum id_type {
				STANDARD_ID = 0,
				EXTENDED_ID
			};
			enum config_type {
				UNCONFIGURED = 0,
				RX,
				TX
			} config;

			Module &mod; // reference to parent, after setup(Module&)
			CAN_CHANNEL chn;
			uint32_t msg_size;
			CAN_CHANNEL_EVENT interrupts;

			CAN_RX_DATA_MODE data_mode;
			CAN_TXCHANNEL_PRIORITY priority;
			CAN_TX_RTR rtr_en;

			Channel(Module &_mod, CAN_CHANNEL _chn = CAN_ALL_CHANNELS, uint32_t _msg_size = 8, // WARNING size
					CAN_CHANNEL_EVENT _inter = (CAN_CHANNEL_EVENT)0);
			Channel(const Channel &a): config(a.config), mod(a.mod), chn(a.chn), msg_size(a.msg_size),
				interrupts(a.interrupts), data_mode(a.data_mode), priority(a.priority), rtr_en(a.rtr_en){}
			virtual ~Channel() {};
			Channel &operator= (const Channel &a) {
				if (this != &a){
					this->Channel::~Channel();
					new (this) Channel(a); // Could use memcpy() instead
				} return *this;
			}

			int32_t setup_rx(CAN_RX_DATA_MODE _data_mode = CAN_RX_FULL_RECEIVE);
			int32_t setup_tx(CAN_TXCHANNEL_PRIORITY _priority, CAN_TX_RTR _rtr = CAN_TX_RTR_DISABLED);
			int32_t add_filter(CAN_FILTER filter, CAN_ID_TYPE f_type, uint32_t id,
								CAN_FILTER_MASK mask, CAN_FILTER_MASK_TYPE mide,
								uint32_t mask_bits);

			int32_t rx(void *dest, uint32_t &id);
			int32_t rx(frame::Packet &p, uint32_t &id);
			int32_t tx(const void *data, size_t num_bytes, uint16_t std_id,
					uint32_t ext_id = 0, id_type type = STANDARD_ID);
			int32_t tx(const frame::Packet &p);
		};


		/**
		 * A CAN Module with 32 channels.
		 * @todo: Control/wrap all RX filters.
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
			static const uint32_t DEFAULT_BUS_SPEED_HZ = 1000000;
			static CAN_BIT_CONFIG default_cfg;

			CAN_MODULE mod;
			char buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES]; // WARNING CAN Message Buffer

			Module(CAN_MODULE _mod = CAN1, uint32_t bus_speed = DEFAULT_BUS_SPEED_HZ,
				   CAN_BIT_CONFIG *timings = &default_cfg,
				   CAN_MODULE_EVENT interrupts = (CAN_MODULE_EVENT)0,
				   INT_PRIORITY int_priority = INT_PRIORITY_DISABLED,
				   CAN_MODULE_FEATURES features = (CAN_MODULE_FEATURES)0);
			operator CAN_MODULE() const {return mod;}

			ALWAYSINLINE Channel& channel(size_t num) {
				return *(Channel *) (channel_buf + num*sizeof(Channel));
			}

			Channel& in () {return channel(0);}
			Channel& out() {return channel(1);}
			Channel& err() {return channel(2);}
			Channel& in1() {return channel(3);}

		private:
			size_t channel_buf[32 * sizeof(Channel)];
			friend struct Channel;
			int32_t ALWAYSINLINE normal_mode() {return switch_mode(CAN_NORMAL_OPERATION, 1);}
			int32_t ALWAYSINLINE config_mode() {return switch_mode(CAN_CONFIGURATION, 1);}
			int32_t switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms);
			int32_t change_features(CAN_MODULE_FEATURES features, BOOL en);
		};
	}
}

#endif

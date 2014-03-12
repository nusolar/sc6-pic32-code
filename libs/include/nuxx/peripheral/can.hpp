#ifndef NUXX_CAN_HPP
#define NUXX_CAN_HPP 1

#include "nuxx/peripheral/can_def.hpp"
#include "nu/compiler.h"
#include <cstdint>
#include <new>
extern "C" {
#include <plib.h>
}

namespace nu {
	namespace Can {
		struct Channel;
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
				NOT_CONFIGURED = 0,
				RX,
				TX
			} config;

			Module &module; // reference to parent, after setup(Module&)
			CAN_CHANNEL chn;
			CAN_CHANNEL_EVENT interrupts;
			CAN_TX_RTR rtr_en;

			Channel(Module &_mod, CAN_CHANNEL _chn = CAN_CHANNEL0, CAN_CHANNEL_EVENT _inter = (CAN_CHANNEL_EVENT)0):
				config(NOT_CONFIGURED), module(_mod), chn(_chn), interrupts(_inter), rtr_en(CAN_TX_RTR_DISABLED) {}
			Channel(const Channel &a):
				config(a.config), module(a.module), chn(a.chn), interrupts(a.interrupts), rtr_en(a.rtr_en){}
			//virtual ~Channel() {}
			Channel &operator= (const Channel &a) {
				if (this != &a){
					this->~Channel();
					new (this) Channel(a); // Could use memcpy() instead
				} return *this;
			}

			int32_t setup_rx(CAN_RX_DATA_MODE _data_mode = CAN_RX_FULL_RECEIVE, uint32_t num_messages = 8);
			int32_t setup_tx(CAN_TXCHANNEL_PRIORITY _priority, CAN_TX_RTR _rtr = CAN_TX_RTR_DISABLED, uint32_t num_messages = 8);
			int32_t link_filter(CAN_FILTER filter, CAN_FILTER_MASK mask);

			int32_t rx(void *dest, uint32_t &id);
			int32_t rx(Packet &p, uint32_t &id);
			int32_t tx(const void *data, size_t num_bytes, uint16_t std_id, uint32_t ext_id = 0, id_type type = STANDARD_ID);
			int32_t tx(const Packet &p);
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
			static CAN_BIT_CONFIG default_cfg;
			static const uint32_t DEFAULT_BUS_SPEED_HZ = 1000000;
			
			CAN_MODULE mod;
			uint32_t bus_speed;
			
			Module(CAN_MODULE _mod = CAN1, uint32_t _bus_speed = DEFAULT_BUS_SPEED_HZ);

			int32_t setup(CAN_BIT_CONFIG *timings = &default_cfg,
						CAN_MODULE_EVENT interrupts = (CAN_MODULE_EVENT)0,
						INT_PRIORITY int_priority = INT_PRIORITY_DISABLED,
						CAN_MODULE_FEATURES features = (CAN_MODULE_FEATURES)0);

			Channel& channel(size_t num) {
				return *(Channel *) (this->channel_buf + num*sizeof(Channel));
			}
			Channel& in () {return this->channel(0);}
			Channel& out() {return this->channel(1);}
			Channel& err() {return this->channel(2);}

			int32_t setup_mask(CAN_FILTER_MASK mask,
							   uint32_t mask_bits,
							   CAN_ID_TYPE _id_type = CAN_SID,
							   CAN_FILTER_MASK_TYPE mide = CAN_FILTER_MASK_IDE_TYPE);
			int32_t setup_filter(CAN_FILTER filter,
								 uint32_t id,
								 CAN_ID_TYPE _id_type = CAN_SID);

		private:
			char buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES]; // WARNING CAN Message Buffer
			size_t channel_buf[32 * sizeof(Channel)];
			friend struct Channel;
			
			int32_t switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms);
			int32_t normal_mode() {return switch_mode(CAN_NORMAL_OPERATION, 1);}
			int32_t config_mode() {return switch_mode(CAN_CONFIGURATION, 1);}
			int32_t change_features(CAN_MODULE_FEATURES features, BOOL en);
		};
	}
}

#endif

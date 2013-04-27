#ifndef NUPP_CAN_HPP
#define NUPP_CAN_HPP 1

#include "nu/compiler.h"
#include <cstdint>
#include <new>
#include <plib.h>

namespace nu {
	namespace can {
		struct Module;

		/**
		 * Base type of RxChannel and TxChannel. Declared before Module so that
		 * a Module can have a Channel[32], but RxChannel and TxChannel
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
		 * CAN Module. Has a default CAN Channel.
		 * TODO: Implement multiple CAN channels.
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

			ALWAYSINLINE Module(CAN_MODULE _mod = CAN1): mod(_mod), rx(Channel(*this)),
				tx(Channel(*this), CAN_LOWEST_PRIORITY), ex(Channel(*this), CAN_LOWEST_PRIORITY) {}
			operator CAN_MODULE() const {return mod;}

			int32_t setup(uint32_t bus_speed = DEFAULT_BUS_SPEED_HZ,
						  CAN_BIT_CONFIG *timings = &default_cfg,
						  CAN_MODULE_EVENT interrupts = (CAN_MODULE_EVENT)0,
						  INT_PRIORITY int_priority = INT_PRIORITY_DISABLED,
						  CAN_MODULE_FEATURES features = (CAN_MODULE_FEATURES)0);

			RxChannel &in() {return rx;}
			TxChannel &out() {return tx;}
			TxChannel &err() {return ex;}

			/** INTRA-NAMESPACE USE ONLY, BUT CAN'T BE PRIVATE */
			int32_t ALWAYSINLINE normal_mode() {return switch_mode(CAN_NORMAL_OPERATION, 1);}
			int32_t ALWAYSINLINE config_mode() {return switch_mode(CAN_CONFIGURATION, 1);}
		private:
			RxChannel rx;
			TxChannel tx, ex;
			int32_t switch_mode(CAN_OP_MODE op_mode, uint32_t timeout_ms);
			int32_t change_features(CAN_MODULE_FEATURES features, BOOL en);
		};


		/**
		 * CAN frames are implemented as PACKED structs,
		 * within namespaces for scoping.
		 */
		namespace frame {
			struct Packet {
				union frame_t {
					uint64_t i;
					uint8_t d[8];
				} frame;
				virtual ~Packet() {}
				virtual uint64_t &get_i() {return frame.i;};
				virtual uint8_t  *get_d() {return frame.d;};
				Packet(): frame{0} {}
				Packet(uint64_t &_i): frame{_i} {get_i() = _i;}
				Packet(Packet& p): frame{p.get_i()} {get_i() = p.get_i();}
				Packet& operator= (uint64_t &_i) {get_i() = _i; return *this;}
				Packet& operator= (Packet& p) {get_i() = p.get_i(); return *this;}
			};

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
			#define Xbase const int base // FUCK MPLAB
			#define X(name, type, ...)\
				; struct name : public Packet {\
					union frame_t {\
						uint64_t i;\
						uint8_t d[8];\
						struct PACKED name##_t {\
							type(__VA_ARGS__)\
						} s;\
					} frame;\
					uint64_t &get_i() {return frame.i;}\
					uint8_t  *get_d() {return frame.d;};\
					name(): frame{0} {}\
					name(uint64_t &_i): frame{_i} {get_i() = _i;}\
					name(Packet& p): frame{p.get_i()} {get_i() = p.get_i();}\
				};
			#define Xinit(name, type, ...) X(name, type, __VA_ARGS__) const int zzz__##name
			#define end ;
				#include "nupp/can.def.hpp"
			#undef end
			#undef Xinit
			#undef X
			#undef Xbase
			#undef List
		}


		/**
		 * CAN addresses are uint16_t's,
		 * within enum classes for scoping.
		 */
		namespace addr {
			#define List(x) enum class x: uint16_t
			#define Xbase base
			#define X(name, type, ...) , name##_k
			#define Xinit(name, type, ...) X(name, type, __VA_ARGS__)
			#define end
				#include "nupp/can.def.hpp"
			#undef end
			#undef Xinit
			#undef X
			#undef Xbase
			#undef List
		}
	}
}

#endif

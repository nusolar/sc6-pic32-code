/*
 * CAN frames are implemented as PACKED structs,
 * within namespaces for scoping.
 *
 * CAN addresses are uint16_t's,
 * within enum classes for scoping.
 */
#include "nu/compiler.h"
#include <cstdint>
#pragma GCC diagnostic ignored "-pedantic"
#define UInt64 uint64_t
#define UInt32 uint32_t
#define UInt16 uint16_t
#define Int32 int32_t
#define Int16 int16_t
#define Byte uint8_t
#define Char int8_t
#define Double double
#define Single float


namespace nu
{
	namespace Can
	{
		struct Packet
		{
			static const uint32_t _id = 0;
			union PACKED frame_t
			{
				uint64_t data;
				uint8_t bytes[8];
			};

		private:
			frame_t _frame;

		protected:
			uint32_t __id;

		public:
			uint32_t id() const {return __id;}
			frame_t &frame() {return _frame;}
			frame_t frame() const {return _frame;}

			//uint64_t data() const {return frame().data;}
			//const uint8_t *bytes() const {return frame().bytes;}
			//uint8_t *bytes() const {return frame().bytes;}

			virtual ~Packet() {}
			Packet(): _frame{0}, __id(_id) {}
			Packet(const uint64_t _data): _frame{_data}, __id(_id) {}
			Packet(const Packet& p): _frame{p.frame().data}, __id(_id) {}
			Packet& operator= (const Packet& p) {frame().data = p.frame().data; return *this;}
		};

		namespace Addr
		{
			namespace bps_rx
			{
				struct trip: Packet
				{
					static const uint32_t _id = 0x200;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 trip_code;
							UInt16 module;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					trip(): Packet(0) {__id = _id;}
					trip(const uint64_t _i): Packet(_i) {__id = _id;}
					trip(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_batt: Packet
				{
					static const uint32_t _id = 0x201;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_batt(): Packet(0) {__id = _id;}
					reset_cc_batt(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_batt(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_array: Packet
				{
					static const uint32_t _id = 0x202;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_array(): Packet(0) {__id = _id;}
					reset_cc_array(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_array(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_mppt1: Packet
				{
					static const uint32_t _id = 0x203;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_mppt1(): Packet(0) {__id = _id;}
					reset_cc_mppt1(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_mppt1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_mppt2: Packet
				{
					static const uint32_t _id = 0x204;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_mppt2(): Packet(0) {__id = _id;}
					reset_cc_mppt2(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_mppt2(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_mppt3: Packet
				{
					static const uint32_t _id = 0x205;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_mppt3(): Packet(0) {__id = _id;}
					reset_cc_mppt3(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_mppt3(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_Wh: Packet
				{
					static const uint32_t _id = 0x206;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_Wh(): Packet(0) {__id = _id;}
					reset_cc_Wh(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_Wh(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cc_all: Packet
				{
					static const uint32_t _id = 0x207;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cc_all(): Packet(0) {__id = _id;}
					reset_cc_all(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cc_all(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace bps_tx
			{
				struct heartbeat: Packet
				{
					static const uint32_t _id = 0x210;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 bmsId;
							UInt32 uptime_s;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					heartbeat(): Packet(0) {__id = _id;}
					heartbeat(const uint64_t _i): Packet(_i) {__id = _id;}
					heartbeat(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct bps_status: Packet
				{
					static const uint32_t _id = 0x211;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 mode;
							UInt16 disabled_module;
							UInt16 last_error;
							UInt16 last_error_value;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					bps_status(): Packet(0) {__id = _id;}
					bps_status(const uint64_t _i): Packet(_i) {__id = _id;}
					bps_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct current: Packet
				{
					static const uint32_t _id = 0x212;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 array;
							UInt16 battery;
							UInt16 reserved;
							UInt16 reserved1;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					current(): Packet(0) {__id = _id;}
					current(const uint64_t _i): Packet(_i) {__id = _id;}
					current(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct voltage_temp: Packet
				{
					static const uint32_t _id = 0x213;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 module;
							UInt16 voltage;
							UInt16 temp;
							UInt16 reserved;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					voltage_temp(): Packet(0) {__id = _id;}
					voltage_temp(const uint64_t _i): Packet(_i) {__id = _id;}
					voltage_temp(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cc_array: Packet
				{
					static const uint32_t _id = 0x214;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cc_array(): Packet(0) {__id = _id;}
					cc_array(const uint64_t _i): Packet(_i) {__id = _id;}
					cc_array(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cc_batt: Packet
				{
					static const uint32_t _id = 0x215;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cc_batt(): Packet(0) {__id = _id;}
					cc_batt(const uint64_t _i): Packet(_i) {__id = _id;}
					cc_batt(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cc_mppt1: Packet
				{
					static const uint32_t _id = 0x216;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cc_mppt1(): Packet(0) {__id = _id;}
					cc_mppt1(const uint64_t _i): Packet(_i) {__id = _id;}
					cc_mppt1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cc_mppt2: Packet
				{
					static const uint32_t _id = 0x217;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cc_mppt2(): Packet(0) {__id = _id;}
					cc_mppt2(const uint64_t _i): Packet(_i) {__id = _id;}
					cc_mppt2(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cc_mppt3: Packet
				{
					static const uint32_t _id = 0x218;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cc_mppt3(): Packet(0) {__id = _id;}
					cc_mppt3(const uint64_t _i): Packet(_i) {__id = _id;}
					cc_mppt3(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct Wh_batt: Packet
				{
					static const uint32_t _id = 0x219;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					Wh_batt(): Packet(0) {__id = _id;}
					Wh_batt(const uint64_t _i): Packet(_i) {__id = _id;}
					Wh_batt(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct Wh_mppt1: Packet
				{
					static const uint32_t _id = 0x21a;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					Wh_mppt1(): Packet(0) {__id = _id;}
					Wh_mppt1(const uint64_t _i): Packet(_i) {__id = _id;}
					Wh_mppt1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct Wh_mppt2: Packet
				{
					static const uint32_t _id = 0x21b;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					Wh_mppt2(): Packet(0) {__id = _id;}
					Wh_mppt2(const uint64_t _i): Packet(_i) {__id = _id;}
					Wh_mppt2(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct Wh_mppt3: Packet
				{
					static const uint32_t _id = 0x21c;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Double count;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					Wh_mppt3(): Packet(0) {__id = _id;}
					Wh_mppt3(const uint64_t _i): Packet(_i) {__id = _id;}
					Wh_mppt3(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct last_trip: Packet
				{
					static const uint32_t _id = 0x21d;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 trip_code;
							UInt16 module;
							UInt16 low_current;
							UInt16 high_current;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					last_trip(): Packet(0) {__id = _id;}
					last_trip(const uint64_t _i): Packet(_i) {__id = _id;}
					last_trip(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct last_trip_voltage_temp: Packet
				{
					static const uint32_t _id = 0x21e;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 low_volt;
							UInt16 high_volt;
							UInt16 low_temp;
							UInt16 high_temp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					last_trip_voltage_temp(): Packet(0) {__id = _id;}
					last_trip_voltage_temp(const uint64_t _i): Packet(_i) {__id = _id;}
					last_trip_voltage_temp(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace os
			{
				struct user_cmds: Packet
				{
					static const uint32_t _id = 0x310;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 power;
							UInt16 gearFlags;
							UInt16 signalFlags;
							UInt16 reserved;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					user_cmds(): Packet(0) {__id = _id;}
					user_cmds(const uint64_t _i): Packet(_i) {__id = _id;}
					user_cmds(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cruise_cmd: Packet
				{
					static const uint32_t _id = 0x311;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single velocity;
							Single current;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cruise_cmd(): Packet(0) {__id = _id;}
					cruise_cmd(const uint64_t _i): Packet(_i) {__id = _id;}
					cruise_cmd(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace ws20
			{
				struct motor_id: Packet
				{
					static const uint32_t _id = 0x400;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 tritiumId;
							UInt32 serialNo;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					motor_id(): Packet(0) {__id = _id;}
					motor_id(const uint64_t _i): Packet(_i) {__id = _id;}
					motor_id(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct motor_status_info: Packet
				{
					static const uint32_t _id = 0x401;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 limitFlags;
							UInt16 errorFlags;
							UInt16 activeMotor;
							UInt16 reserved;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					motor_status_info(): Packet(0) {__id = _id;}
					motor_status_info(const uint64_t _i): Packet(_i) {__id = _id;}
					motor_status_info(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct motor_bus: Packet
				{
					static const uint32_t _id = 0x402;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single busVoltage;
							Single busCurrent;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					motor_bus(): Packet(0) {__id = _id;}
					motor_bus(const uint64_t _i): Packet(_i) {__id = _id;}
					motor_bus(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct motor_velocity: Packet
				{
					static const uint32_t _id = 0x403;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single motorVelocity;
							Single vehicleVelocity;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					motor_velocity(): Packet(0) {__id = _id;}
					motor_velocity(const uint64_t _i): Packet(_i) {__id = _id;}
					motor_velocity(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct motor_phase: Packet
				{
					static const uint32_t _id = 0x404;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single phaseBCurrent;
							Single phaseACurrent;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					motor_phase(): Packet(0) {__id = _id;}
					motor_phase(const uint64_t _i): Packet(_i) {__id = _id;}
					motor_phase(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct voltage_vector: Packet
				{
					static const uint32_t _id = 0x405;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single voltageIm;
							Single voltageRe;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					voltage_vector(): Packet(0) {__id = _id;}
					voltage_vector(const uint64_t _i): Packet(_i) {__id = _id;}
					voltage_vector(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct current_vector: Packet
				{
					static const uint32_t _id = 0x406;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single currentIm;
							Single currentRe;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					current_vector(): Packet(0) {__id = _id;}
					current_vector(const uint64_t _i): Packet(_i) {__id = _id;}
					current_vector(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct backemf: Packet
				{
					static const uint32_t _id = 0x407;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single backEmfIm;
							Single backEmfRe;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					backemf(): Packet(0) {__id = _id;}
					backemf(const uint64_t _i): Packet(_i) {__id = _id;}
					backemf(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct rail_15v_1pt65v: Packet
				{
					static const uint32_t _id = 0x408;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single onePtSixtyFiveVRef;
							Single fifteenVPowerRail;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					rail_15v_1pt65v(): Packet(0) {__id = _id;}
					rail_15v_1pt65v(const uint64_t _i): Packet(_i) {__id = _id;}
					rail_15v_1pt65v(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct rail_2pt5v_1pt2v: Packet
				{
					static const uint32_t _id = 0x409;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single onePtTwoVSupply;
							Single twoPtFiveVSupply;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					rail_2pt5v_1pt2v(): Packet(0) {__id = _id;}
					rail_2pt5v_1pt2v(const uint64_t _i): Packet(_i) {__id = _id;}
					rail_2pt5v_1pt2v(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct fanspeed: Packet
				{
					static const uint32_t _id = 0x40a;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single fanDrive;
							Single fanRpm;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					fanspeed(): Packet(0) {__id = _id;}
					fanspeed(const uint64_t _i): Packet(_i) {__id = _id;}
					fanspeed(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct sinks_temp: Packet
				{
					static const uint32_t _id = 0x40b;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single motorTemp;
							Single heatsinkTemp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					sinks_temp(): Packet(0) {__id = _id;}
					sinks_temp(const uint64_t _i): Packet(_i) {__id = _id;}
					sinks_temp(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cpu_airin_temp: Packet
				{
					static const uint32_t _id = 0x40c;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single processorTemp;
							Single airInletTemp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cpu_airin_temp(): Packet(0) {__id = _id;}
					cpu_airin_temp(const uint64_t _i): Packet(_i) {__id = _id;}
					cpu_airin_temp(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cap_airout_temp: Packet
				{
					static const uint32_t _id = 0x40d;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single capacitorTemp;
							Single airOutTemp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cap_airout_temp(): Packet(0) {__id = _id;}
					cap_airout_temp(const uint64_t _i): Packet(_i) {__id = _id;}
					cap_airout_temp(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct odom_bus_ah: Packet
				{
					static const uint32_t _id = 0x40e;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single odom;
							Single dcBusAmpHours;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					odom_bus_ah(): Packet(0) {__id = _id;}
					odom_bus_ah(const uint64_t _i): Packet(_i) {__id = _id;}
					odom_bus_ah(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace dc
			{
				struct driver_controls_id: Packet
				{
					static const uint32_t _id = 0x500;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 drvId;
							UInt32 serialNo;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					driver_controls_id(): Packet(0) {__id = _id;}
					driver_controls_id(const uint64_t _i): Packet(_i) {__id = _id;}
					driver_controls_id(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct drive_cmd: Packet
				{
					static const uint32_t _id = 0x501;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single motorVelocity;
							Single motorCurrent;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					drive_cmd(): Packet(0) {__id = _id;}
					drive_cmd(const uint64_t _i): Packet(_i) {__id = _id;}
					drive_cmd(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct power_cmd: Packet
				{
					static const uint32_t _id = 0x502;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single reserved;
							Single busCurrent;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					power_cmd(): Packet(0) {__id = _id;}
					power_cmd(const uint64_t _i): Packet(_i) {__id = _id;}
					power_cmd(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reset_cmd: Packet
				{
					static const uint32_t _id = 0x503;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 unused0;
							UInt32 unused1;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cmd(): Packet(0) {__id = _id;}
					reset_cmd(const uint64_t _i): Packet(_i) {__id = _id;}
					reset_cmd(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct unused0: Packet
				{
					static const uint32_t _id = 0x504;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					unused0(): Packet(0) {__id = _id;}
					unused0(const uint64_t _i): Packet(_i) {__id = _id;}
					unused0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct switches: Packet
				{
					static const uint32_t _id = 0x505;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 switchFlags;
							UInt16 unused0;
							UInt16 unused1;
							UInt16 unused2;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					switches(): Packet(0) {__id = _id;}
					switches(const uint64_t _i): Packet(_i) {__id = _id;}
					switches(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct pedals: Packet
				{
					static const uint32_t _id = 0x506;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 accel_pedal;
							UInt16 regen_pedal;
							UInt16 brake_pedal;
							UInt16 reserved;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pedals(): Packet(0) {__id = _id;}
					pedals(const uint64_t _i): Packet(_i) {__id = _id;}
					pedals(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace bms0
			{
				struct heartbeat: Packet
				{
					static const uint32_t _id = 0x600;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 bmsId;
							UInt32 serialNo;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					heartbeat(): Packet(0) {__id = _id;}
					heartbeat(const uint64_t _i): Packet(_i) {__id = _id;}
					heartbeat(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu1_status: Packet
				{
					static const uint32_t _id = 0x601;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 serialNo0;
							Int16 serialNo1;
							Int16 pcb_temp;
							Int16 cell_temp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu1_status(): Packet(0) {__id = _id;}
					cmu1_status(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu1_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu1_volts0: Packet
				{
					static const uint32_t _id = 0x602;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell0;
							Int16 cell1;
							Int16 cell2;
							Int16 cell3;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu1_volts0(): Packet(0) {__id = _id;}
					cmu1_volts0(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu1_volts0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu1_volts1: Packet
				{
					static const uint32_t _id = 0x603;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell4;
							Int16 cell5;
							Int16 cell6;
							Int16 cell7;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu1_volts1(): Packet(0) {__id = _id;}
					cmu1_volts1(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu1_volts1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu2_status: Packet
				{
					static const uint32_t _id = 0x604;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 serialNo0;
							Int16 serialNo1;
							Int16 pcb_temp;
							Int16 cell_temp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu2_status(): Packet(0) {__id = _id;}
					cmu2_status(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu2_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu2_volts0: Packet
				{
					static const uint32_t _id = 0x605;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell0;
							Int16 cell1;
							Int16 cell2;
							Int16 cell3;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu2_volts0(): Packet(0) {__id = _id;}
					cmu2_volts0(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu2_volts0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu2_volts1: Packet
				{
					static const uint32_t _id = 0x606;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell4;
							Int16 cell5;
							Int16 cell6;
							Int16 cell7;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu2_volts1(): Packet(0) {__id = _id;}
					cmu2_volts1(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu2_volts1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu3_status: Packet
				{
					static const uint32_t _id = 0x607;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 serialNo0;
							Int16 serialNo1;
							Int16 pcb_temp;
							Int16 cell_temp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu3_status(): Packet(0) {__id = _id;}
					cmu3_status(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu3_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu3_volts0: Packet
				{
					static const uint32_t _id = 0x608;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell0;
							Int16 cell1;
							Int16 cell2;
							Int16 cell3;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu3_volts0(): Packet(0) {__id = _id;}
					cmu3_volts0(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu3_volts0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu3_volts1: Packet
				{
					static const uint32_t _id = 0x609;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell4;
							Int16 cell5;
							Int16 cell6;
							Int16 cell7;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu3_volts1(): Packet(0) {__id = _id;}
					cmu3_volts1(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu3_volts1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu4_status: Packet
				{
					static const uint32_t _id = 0x60a;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 serialNo0;
							Int16 serialNo1;
							Int16 pcb_temp;
							Int16 cell_temp;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu4_status(): Packet(0) {__id = _id;}
					cmu4_status(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu4_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu4_volts0: Packet
				{
					static const uint32_t _id = 0x60b;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell0;
							Int16 cell1;
							Int16 cell2;
							Int16 cell3;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu4_volts0(): Packet(0) {__id = _id;}
					cmu4_volts0(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu4_volts0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct cmu4_volts1: Packet
				{
					static const uint32_t _id = 0x60c;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 cell4;
							Int16 cell5;
							Int16 cell6;
							Int16 cell7;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu4_volts1(): Packet(0) {__id = _id;}
					cmu4_volts1(const uint64_t _i): Packet(_i) {__id = _id;}
					cmu4_volts1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace bms1
			{
				struct reserved0: Packet
				{
					static const uint32_t _id = 0x6f0;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reserved0(): Packet(0) {__id = _id;}
					reserved0(const uint64_t _i): Packet(_i) {__id = _id;}
					reserved0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reserved1: Packet
				{
					static const uint32_t _id = 0x6f1;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reserved1(): Packet(0) {__id = _id;}
					reserved1(const uint64_t _i): Packet(_i) {__id = _id;}
					reserved1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reserved2: Packet
				{
					static const uint32_t _id = 0x6f2;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reserved2(): Packet(0) {__id = _id;}
					reserved2(const uint64_t _i): Packet(_i) {__id = _id;}
					reserved2(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct reserved3: Packet
				{
					static const uint32_t _id = 0x6f3;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reserved3(): Packet(0) {__id = _id;}
					reserved3(const uint64_t _i): Packet(_i) {__id = _id;}
					reserved3(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct pack_soc: Packet
				{
					static const uint32_t _id = 0x6f4;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single soc_Ah;
							Single soc_percentage;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_soc(): Packet(0) {__id = _id;}
					pack_soc(const uint64_t _i): Packet(_i) {__id = _id;}
					pack_soc(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct pack_bal_soc: Packet
				{
					static const uint32_t _id = 0x6f5;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single soc_Ah;
							Single soc_percentage;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_bal_soc(): Packet(0) {__id = _id;}
					pack_bal_soc(const uint64_t _i): Packet(_i) {__id = _id;}
					pack_bal_soc(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct charger_cmd: Packet
				{
					static const uint32_t _id = 0x6f6;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int16 charging_mV_err;
							Int16 temp_margin;
							Int16 discharging_mV_err;
							Int16 pack_capacity_Ah;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					charger_cmd(): Packet(0) {__id = _id;}
					charger_cmd(const uint64_t _i): Packet(_i) {__id = _id;}
					charger_cmd(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct precharge: Packet
				{
					static const uint32_t _id = 0x6f7;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Byte precharge_flags;
							Byte precharge_state;
							Byte unused0;
							Byte unused1;
							Byte unused2;
							Byte unused3;
							Byte precharge_timer_flags;
							Byte precharge_timer_10ms;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					precharge(): Packet(0) {__id = _id;}
					precharge(const uint64_t _i): Packet(_i) {__id = _id;}
					precharge(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct max_min_volts: Packet
				{
					static const uint32_t _id = 0x6f8;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					max_min_volts(): Packet(0) {__id = _id;}
					max_min_volts(const uint64_t _i): Packet(_i) {__id = _id;}
					max_min_volts(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct max_min_temps: Packet
				{
					static const uint32_t _id = 0x6f9;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					max_min_temps(): Packet(0) {__id = _id;}
					max_min_temps(const uint64_t _i): Packet(_i) {__id = _id;}
					max_min_temps(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct pack_volt_curr: Packet
				{
					static const uint32_t _id = 0x6fa;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int32 pack_voltage;
							Int32 pack_current;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_volt_curr(): Packet(0) {__id = _id;}
					pack_volt_curr(const uint64_t _i): Packet(_i) {__id = _id;}
					pack_volt_curr(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct pack_status: Packet
				{
					static const uint32_t _id = 0x6fb;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 thresh_rising;
							UInt16 thresh_falling;
							UInt16 pack_flags;
							UInt16 firmwareNo;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_status(): Packet(0) {__id = _id;}
					pack_status(const uint64_t _i): Packet(_i) {__id = _id;}
					pack_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct fan_status: Packet
				{
					static const uint32_t _id = 0x6fc;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 fan0_rpm;
							UInt16 fan1_rpm;
							UInt16 fans_relays_mA;
							UInt16 cmus_mA;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					fan_status(): Packet(0) {__id = _id;}
					fan_status(const uint64_t _i): Packet(_i) {__id = _id;}
					fan_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct extended_status: Packet
				{
					static const uint32_t _id = 0x6fd;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 pack_flags0;
							UInt16 pack_flags1;
							UInt16 hardware_model_id;
							UInt16 unused0;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					extended_status(): Packet(0) {__id = _id;}
					extended_status(const uint64_t _i): Packet(_i) {__id = _id;}
					extended_status(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace mppt_master
			{
				struct unused0: Packet
				{
					static const uint32_t _id = 0x710;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					unused0(): Packet(0) {__id = _id;}
					unused0(const uint64_t _i): Packet(_i) {__id = _id;}
					unused0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct mppt1: Packet
				{
					static const uint32_t _id = 0x711;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt1(): Packet(0) {__id = _id;}
					mppt1(const uint64_t _i): Packet(_i) {__id = _id;}
					mppt1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct mppt2: Packet
				{
					static const uint32_t _id = 0x712;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt2(): Packet(0) {__id = _id;}
					mppt2(const uint64_t _i): Packet(_i) {__id = _id;}
					mppt2(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct mppt3: Packet
				{
					static const uint32_t _id = 0x713;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt3(): Packet(0) {__id = _id;}
					mppt3(const uint64_t _i): Packet(_i) {__id = _id;}
					mppt3(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}

			namespace mppt
			{
				struct unused0: Packet
				{
					static const uint32_t _id = 0x770;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{

						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					unused0(): Packet(0) {__id = _id;}
					unused0(const uint64_t _i): Packet(_i) {__id = _id;}
					unused0(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct mppt1: Packet
				{
					static const uint32_t _id = 0x771;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 flags_Vin;
							UInt16 flags_Iin;
							UInt16 flags_Vout;
							UInt16 flags_Tout;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt1(): Packet(0) {__id = _id;}
					mppt1(const uint64_t _i): Packet(_i) {__id = _id;}
					mppt1(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct mppt2: Packet
				{
					static const uint32_t _id = 0x772;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 flags_Vin;
							UInt16 flags_Iin;
							UInt16 flags_Vout;
							UInt16 flags_Tout;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt2(): Packet(0) {__id = _id;}
					mppt2(const uint64_t _i): Packet(_i) {__id = _id;}
					mppt2(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};

				struct mppt3: Packet
				{
					static const uint32_t _id = 0x773;
					union PACKED layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 flags_Vin;
							UInt16 flags_Iin;
							UInt16 flags_Vout;
							UInt16 flags_Tout;
						};
					};
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt3(): Packet(0) {__id = _id;}
					mppt3(const uint64_t _i): Packet(_i) {__id = _id;}
					mppt3(const Packet& p): Packet(p.frame().data) {__id = _id;}
				};
			}
		}
	}
}


#undef UInt64
#undef UInt32
#undef UInt16
#undef Int32
#undef Int16
#undef Byte
#undef Char
#undef Double
#undef Single
#pragma GCC diagnostic warning "-pedantic"



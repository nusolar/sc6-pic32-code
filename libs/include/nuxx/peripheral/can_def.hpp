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
			union frame_t
			{
				uint64_t data;
				uint8_t bytes[8];
			};

		private:
			static const uint32_t _id = 0;
			frame_t _frame;

		public:
			virtual uint32_t id() const {return _id;}
			frame_t &frame() {return _frame;}
			frame_t frame() const {return _frame;}

			uint64_t &data() {return frame().data;}
			uint64_t data() const {return frame().data;}

			uint8_t *bytes() {return frame().bytes;}
			const uint8_t *bytes() const {return frame().bytes;}

			virtual ~Packet() {}
			Packet(): _frame{0} {}
			Packet(const uint64_t _data): _frame{_data} {}
			Packet(const Packet& p): _frame{p.data()} {}
			Packet& operator= (const uint64_t _data) {data() = _data; return *this;}
			Packet& operator= (const Packet& p) {data() = p.data(); return *this;}
		};

		namespace Addr
		{
			namespace bps
			{
				namespace rx
				{
					enum addresses
					{
						trip_k = 0x200,
						reset_cc_batt_k = 0x201,
						reset_cc_array_k = 0x202,
						reset_cc_mppt1_k = 0x203,
						reset_cc_mppt2_k = 0x204,
						reset_cc_mppt3_k = 0x205,
						reset_cc_Wh_k = 0x206,
						reset_cc_all_k = 0x207
					};

					class trip: public Packet
					{
						static const uint32_t _id = 0x200;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Int16 trip_code;
								UInt16 module;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						trip(): Packet(0) {}
						trip(const uint64_t _i): Packet(_i) {}
						trip(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_batt: public Packet
					{
						static const uint32_t _id = 0x201;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_batt(): Packet(0) {}
						reset_cc_batt(const uint64_t _i): Packet(_i) {}
						reset_cc_batt(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_array: public Packet
					{
						static const uint32_t _id = 0x202;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_array(): Packet(0) {}
						reset_cc_array(const uint64_t _i): Packet(_i) {}
						reset_cc_array(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_mppt1: public Packet
					{
						static const uint32_t _id = 0x203;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_mppt1(): Packet(0) {}
						reset_cc_mppt1(const uint64_t _i): Packet(_i) {}
						reset_cc_mppt1(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_mppt2: public Packet
					{
						static const uint32_t _id = 0x204;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_mppt2(): Packet(0) {}
						reset_cc_mppt2(const uint64_t _i): Packet(_i) {}
						reset_cc_mppt2(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_mppt3: public Packet
					{
						static const uint32_t _id = 0x205;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_mppt3(): Packet(0) {}
						reset_cc_mppt3(const uint64_t _i): Packet(_i) {}
						reset_cc_mppt3(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_Wh: public Packet
					{
						static const uint32_t _id = 0x206;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_Wh(): Packet(0) {}
						reset_cc_Wh(const uint64_t _i): Packet(_i) {}
						reset_cc_Wh(const Packet& p): Packet(p.data()) {}
					};

					class reset_cc_all: public Packet
					{
						static const uint32_t _id = 0x207;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cc_all(): Packet(0) {}
						reset_cc_all(const uint64_t _i): Packet(_i) {}
						reset_cc_all(const Packet& p): Packet(p.data()) {}
					};
				}

				namespace tx
				{
					enum addresses
					{
						heartbeat_k = 0x210,
						bps_status_k = 0x211,
						current_k = 0x212,
						voltage_temp_k = 0x213,
						cc_array_k = 0x214,
						cc_batt_k = 0x215,
						cc_mppt1_k = 0x216,
						cc_mppt2_k = 0x217,
						cc_mppt3_k = 0x218,
						Wh_batt_k = 0x219,
						Wh_mppt1_k = 0x21a,
						Wh_mppt2_k = 0x21b,
						Wh_mppt3_k = 0x21c,
						last_trip_k = 0x21d,
						last_trip_voltage_temp_k = 0x21e
					};

					class heartbeat: public Packet
					{
						static const uint32_t _id = 0x210;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								UInt32 bms_str;
								UInt32 uptime_s;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						heartbeat(): Packet(0) {}
						heartbeat(const uint64_t _i): Packet(_i) {}
						heartbeat(const Packet& p): Packet(p.data()) {}
					};

					class bps_status: public Packet
					{
						static const uint32_t _id = 0x211;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						bps_status(): Packet(0) {}
						bps_status(const uint64_t _i): Packet(_i) {}
						bps_status(const Packet& p): Packet(p.data()) {}
					};

					class current: public Packet
					{
						static const uint32_t _id = 0x212;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						current(): Packet(0) {}
						current(const uint64_t _i): Packet(_i) {}
						current(const Packet& p): Packet(p.data()) {}
					};

					class voltage_temp: public Packet
					{
						static const uint32_t _id = 0x213;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						voltage_temp(): Packet(0) {}
						voltage_temp(const uint64_t _i): Packet(_i) {}
						voltage_temp(const Packet& p): Packet(p.data()) {}
					};

					class cc_array: public Packet
					{
						static const uint32_t _id = 0x214;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cc_array(): Packet(0) {}
						cc_array(const uint64_t _i): Packet(_i) {}
						cc_array(const Packet& p): Packet(p.data()) {}
					};

					class cc_batt: public Packet
					{
						static const uint32_t _id = 0x215;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cc_batt(): Packet(0) {}
						cc_batt(const uint64_t _i): Packet(_i) {}
						cc_batt(const Packet& p): Packet(p.data()) {}
					};

					class cc_mppt1: public Packet
					{
						static const uint32_t _id = 0x216;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cc_mppt1(): Packet(0) {}
						cc_mppt1(const uint64_t _i): Packet(_i) {}
						cc_mppt1(const Packet& p): Packet(p.data()) {}
					};

					class cc_mppt2: public Packet
					{
						static const uint32_t _id = 0x217;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cc_mppt2(): Packet(0) {}
						cc_mppt2(const uint64_t _i): Packet(_i) {}
						cc_mppt2(const Packet& p): Packet(p.data()) {}
					};

					class cc_mppt3: public Packet
					{
						static const uint32_t _id = 0x218;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cc_mppt3(): Packet(0) {}
						cc_mppt3(const uint64_t _i): Packet(_i) {}
						cc_mppt3(const Packet& p): Packet(p.data()) {}
					};

					class Wh_batt: public Packet
					{
						static const uint32_t _id = 0x219;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						Wh_batt(): Packet(0) {}
						Wh_batt(const uint64_t _i): Packet(_i) {}
						Wh_batt(const Packet& p): Packet(p.data()) {}
					};

					class Wh_mppt1: public Packet
					{
						static const uint32_t _id = 0x21a;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						Wh_mppt1(): Packet(0) {}
						Wh_mppt1(const uint64_t _i): Packet(_i) {}
						Wh_mppt1(const Packet& p): Packet(p.data()) {}
					};

					class Wh_mppt2: public Packet
					{
						static const uint32_t _id = 0x21b;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						Wh_mppt2(): Packet(0) {}
						Wh_mppt2(const uint64_t _i): Packet(_i) {}
						Wh_mppt2(const Packet& p): Packet(p.data()) {}
					};

					class Wh_mppt3: public Packet
					{
						static const uint32_t _id = 0x21c;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						Wh_mppt3(): Packet(0) {}
						Wh_mppt3(const uint64_t _i): Packet(_i) {}
						Wh_mppt3(const Packet& p): Packet(p.data()) {}
					};

					class last_trip: public Packet
					{
						static const uint32_t _id = 0x21d;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						last_trip(): Packet(0) {}
						last_trip(const uint64_t _i): Packet(_i) {}
						last_trip(const Packet& p): Packet(p.data()) {}
					};

					class last_trip_voltage_temp: public Packet
					{
						static const uint32_t _id = 0x21e;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						last_trip_voltage_temp(): Packet(0) {}
						last_trip_voltage_temp(const uint64_t _i): Packet(_i) {}
						last_trip_voltage_temp(const Packet& p): Packet(p.data()) {}
					};
				}
			}

			namespace ws20
			{
				namespace rx
				{
					enum addresses
					{
						driver_controls_id_k = 0x500,
						drive_cmd_k = 0x501,
						power_cmd_k = 0x502,
						reset_cmd_k = 0x503
					};

					class driver_controls_id: public Packet
					{
						static const uint32_t _id = 0x500;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								UInt32 drvId;
								UInt32 serialNo;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						driver_controls_id(): Packet(0) {}
						driver_controls_id(const uint64_t _i): Packet(_i) {}
						driver_controls_id(const Packet& p): Packet(p.data()) {}
					};

					class drive_cmd: public Packet
					{
						static const uint32_t _id = 0x501;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single motorVelocity;
								Single motorCurrent;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						drive_cmd(): Packet(0) {}
						drive_cmd(const uint64_t _i): Packet(_i) {}
						drive_cmd(const Packet& p): Packet(p.data()) {}
					};

					class power_cmd: public Packet
					{
						static const uint32_t _id = 0x502;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single reserved;
								Single busCurrent;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						power_cmd(): Packet(0) {}
						power_cmd(const uint64_t _i): Packet(_i) {}
						power_cmd(const Packet& p): Packet(p.data()) {}
					};

					class reset_cmd: public Packet
					{
						static const uint32_t _id = 0x503;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								UInt32 unused1;
								UInt32 unused2;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						reset_cmd(): Packet(0) {}
						reset_cmd(const uint64_t _i): Packet(_i) {}
						reset_cmd(const Packet& p): Packet(p.data()) {}
					};
				}

				namespace tx
				{
					enum addresses
					{
						motor_id_k = 0x400,
						motor_status_info_k = 0x401,
						motor_bus_k = 0x402,
						motor_velocity_k = 0x403,
						motor_phase_k = 0x404,
						voltage_vector_k = 0x405,
						current_vector_k = 0x406,
						backemf_k = 0x407,
						rail_15v_1pt65v_k = 0x408,
						rail_2pt5v_1pt2v_k = 0x409,
						fanspeed_k = 0x40a,
						sinks_temp_k = 0x40b,
						cpu_airin_temp_k = 0x40c,
						cap_airout_temp_k = 0x40d,
						odom_bus_ah_k = 0x40e
					};

					class motor_id: public Packet
					{
						static const uint32_t _id = 0x400;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								UInt32 tritiumId;
								UInt32 serialNo;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						motor_id(): Packet(0) {}
						motor_id(const uint64_t _i): Packet(_i) {}
						motor_id(const Packet& p): Packet(p.data()) {}
					};

					class motor_status_info: public Packet
					{
						static const uint32_t _id = 0x401;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						motor_status_info(): Packet(0) {}
						motor_status_info(const uint64_t _i): Packet(_i) {}
						motor_status_info(const Packet& p): Packet(p.data()) {}
					};

					class motor_bus: public Packet
					{
						static const uint32_t _id = 0x402;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single busVoltage;
								Single busCurrent;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						motor_bus(): Packet(0) {}
						motor_bus(const uint64_t _i): Packet(_i) {}
						motor_bus(const Packet& p): Packet(p.data()) {}
					};

					class motor_velocity: public Packet
					{
						static const uint32_t _id = 0x403;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single motorVelocity;
								Single vehicleVelocity;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						motor_velocity(): Packet(0) {}
						motor_velocity(const uint64_t _i): Packet(_i) {}
						motor_velocity(const Packet& p): Packet(p.data()) {}
					};

					class motor_phase: public Packet
					{
						static const uint32_t _id = 0x404;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single phaseBCurrent;
								Single phaseACurrent;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						motor_phase(): Packet(0) {}
						motor_phase(const uint64_t _i): Packet(_i) {}
						motor_phase(const Packet& p): Packet(p.data()) {}
					};

					class voltage_vector: public Packet
					{
						static const uint32_t _id = 0x405;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single voltageIm;
								Single voltageRe;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						voltage_vector(): Packet(0) {}
						voltage_vector(const uint64_t _i): Packet(_i) {}
						voltage_vector(const Packet& p): Packet(p.data()) {}
					};

					class current_vector: public Packet
					{
						static const uint32_t _id = 0x406;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single currentIm;
								Single currentRe;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						current_vector(): Packet(0) {}
						current_vector(const uint64_t _i): Packet(_i) {}
						current_vector(const Packet& p): Packet(p.data()) {}
					};

					class backemf: public Packet
					{
						static const uint32_t _id = 0x407;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single backEmfIm;
								Single backEmfRe;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						backemf(): Packet(0) {}
						backemf(const uint64_t _i): Packet(_i) {}
						backemf(const Packet& p): Packet(p.data()) {}
					};

					class rail_15v_1pt65v: public Packet
					{
						static const uint32_t _id = 0x408;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single onePtSixtyFiveVRef;
								Single fifteenVPowerRail;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						rail_15v_1pt65v(): Packet(0) {}
						rail_15v_1pt65v(const uint64_t _i): Packet(_i) {}
						rail_15v_1pt65v(const Packet& p): Packet(p.data()) {}
					};

					class rail_2pt5v_1pt2v: public Packet
					{
						static const uint32_t _id = 0x409;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single onePtTwoVSupply;
								Single twoPtFiveVSupply;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						rail_2pt5v_1pt2v(): Packet(0) {}
						rail_2pt5v_1pt2v(const uint64_t _i): Packet(_i) {}
						rail_2pt5v_1pt2v(const Packet& p): Packet(p.data()) {}
					};

					class fanspeed: public Packet
					{
						static const uint32_t _id = 0x40a;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single fanDrive;
								Single fanRpm;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						fanspeed(): Packet(0) {}
						fanspeed(const uint64_t _i): Packet(_i) {}
						fanspeed(const Packet& p): Packet(p.data()) {}
					};

					class sinks_temp: public Packet
					{
						static const uint32_t _id = 0x40b;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single motorTemp;
								Single heatsinkTemp;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						sinks_temp(): Packet(0) {}
						sinks_temp(const uint64_t _i): Packet(_i) {}
						sinks_temp(const Packet& p): Packet(p.data()) {}
					};

					class cpu_airin_temp: public Packet
					{
						static const uint32_t _id = 0x40c;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single processorTemp;
								Single airInletTemp;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cpu_airin_temp(): Packet(0) {}
						cpu_airin_temp(const uint64_t _i): Packet(_i) {}
						cpu_airin_temp(const Packet& p): Packet(p.data()) {}
					};

					class cap_airout_temp: public Packet
					{
						static const uint32_t _id = 0x40d;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single capacitorTemp;
								Single airOutTemp;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cap_airout_temp(): Packet(0) {}
						cap_airout_temp(const uint64_t _i): Packet(_i) {}
						cap_airout_temp(const Packet& p): Packet(p.data()) {}
					};

					class odom_bus_ah: public Packet
					{
						static const uint32_t _id = 0x40e;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single odom;
								Single dcBusAmpHours;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						odom_bus_ah(): Packet(0) {}
						odom_bus_ah(const uint64_t _i): Packet(_i) {}
						odom_bus_ah(const Packet& p): Packet(p.data()) {}
					};
				}
			}

			namespace pedals
			{
				namespace rx
				{
					enum addresses
					{
						cruise_k = 0x100
					};

					class cruise: public Packet
					{
						static const uint32_t _id = 0x100;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								Single velocity;
								Single current;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						cruise(): Packet(0) {}
						cruise(const uint64_t _i): Packet(_i) {}
						cruise(const Packet& p): Packet(p.data()) {}
					};
				}

				namespace tx
				{
					enum addresses
					{
						pedals_k = 0x110
					};

					class pedals: public Packet
					{
						static const uint32_t _id = 0x110;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								UInt16 accel_pedal;
								UInt16 regen_pedal;
								UInt16 brake_pedal;
								UInt16 reserved1;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						pedals(): Packet(0) {}
						pedals(const uint64_t _i): Packet(_i) {}
						pedals(const Packet& p): Packet(p.data()) {}
					};
				}
			}

			namespace os
			{
				namespace tx
				{
					enum addresses
					{
						user_cmds_k = 0x310
					};

					class user_cmds: public Packet
					{
						static const uint32_t _id = 0x310;
					public:
						union layout_t
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
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						user_cmds(): Packet(0) {}
						user_cmds(const uint64_t _i): Packet(_i) {}
						user_cmds(const Packet& p): Packet(p.data()) {}
					};
				}
			}

			namespace mppt
			{
				namespace rx
				{
					enum addresses
					{
						mppt_k = 0x710
					};

					class mppt: public Packet
					{
						static const uint32_t _id = 0x710;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						mppt(): Packet(0) {}
						mppt(const uint64_t _i): Packet(_i) {}
						mppt(const Packet& p): Packet(p.data()) {}
					};
				}

				namespace tx
				{
					enum addresses
					{
						mppt_k = 0x710
					};

					class mppt: public Packet
					{
						static const uint32_t _id = 0x710;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{

							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						mppt(): Packet(0) {}
						mppt(const uint64_t _i): Packet(_i) {}
						mppt(const Packet& p): Packet(p.data()) {}
					};
				}
			}
		}
	}
}


#undef UInt64
#undef UInt32
#undef UInt16
#undef Int16
#undef Byte
#undef Char
#undef Double
#undef Single
#pragma GCC diagnostic warning "-pedantic"



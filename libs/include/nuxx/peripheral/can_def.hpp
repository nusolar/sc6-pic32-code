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
			namespace bps_rx
			{
				struct trip: Packet
				{
					static const uint32_t _id = 0x200;
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

				struct reset_cc_batt: Packet
				{
					static const uint32_t _id = 0x201;
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

				struct reset_cc_array: Packet
				{
					static const uint32_t _id = 0x202;
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

				struct reset_cc_mppt1: Packet
				{
					static const uint32_t _id = 0x203;
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

				struct reset_cc_mppt2: Packet
				{
					static const uint32_t _id = 0x204;
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

				struct reset_cc_mppt3: Packet
				{
					static const uint32_t _id = 0x205;
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

				struct reset_cc_Wh: Packet
				{
					static const uint32_t _id = 0x206;
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

				struct reset_cc_all: Packet
				{
					static const uint32_t _id = 0x207;
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

			namespace bps_tx
			{
				struct heartbeat: Packet
				{
					static const uint32_t _id = 0x210;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 bmsId;
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

				struct bps_status: Packet
				{
					static const uint32_t _id = 0x211;
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

				struct current: Packet
				{
					static const uint32_t _id = 0x212;
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

				struct voltage_temp: Packet
				{
					static const uint32_t _id = 0x213;
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

				struct cc_array: Packet
				{
					static const uint32_t _id = 0x214;
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

				struct cc_batt: Packet
				{
					static const uint32_t _id = 0x215;
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

				struct cc_mppt1: Packet
				{
					static const uint32_t _id = 0x216;
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

				struct cc_mppt2: Packet
				{
					static const uint32_t _id = 0x217;
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

				struct cc_mppt3: Packet
				{
					static const uint32_t _id = 0x218;
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

				struct Wh_batt: Packet
				{
					static const uint32_t _id = 0x219;
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

				struct Wh_mppt1: Packet
				{
					static const uint32_t _id = 0x21a;
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

				struct Wh_mppt2: Packet
				{
					static const uint32_t _id = 0x21b;
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

				struct Wh_mppt3: Packet
				{
					static const uint32_t _id = 0x21c;
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

				struct last_trip: Packet
				{
					static const uint32_t _id = 0x21d;
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

				struct last_trip_voltage_temp: Packet
				{
					static const uint32_t _id = 0x21e;
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

			namespace bms0
			{
				struct heartbeat: Packet
				{
					static const uint32_t _id = 0x600;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 bmsId;
							UInt32 serialNo;
						};
					};
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					heartbeat(): Packet(0) {}
					heartbeat(const uint64_t _i): Packet(_i) {}
					heartbeat(const Packet& p): Packet(p.data()) {}
				};

				struct cmu1_status: Packet
				{
					static const uint32_t _id = 0x601;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu1_status(): Packet(0) {}
					cmu1_status(const uint64_t _i): Packet(_i) {}
					cmu1_status(const Packet& p): Packet(p.data()) {}
				};

				struct cmu1_volts0: Packet
				{
					static const uint32_t _id = 0x602;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu1_volts0(): Packet(0) {}
					cmu1_volts0(const uint64_t _i): Packet(_i) {}
					cmu1_volts0(const Packet& p): Packet(p.data()) {}
				};

				struct cmu1_volts1: Packet
				{
					static const uint32_t _id = 0x603;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu1_volts1(): Packet(0) {}
					cmu1_volts1(const uint64_t _i): Packet(_i) {}
					cmu1_volts1(const Packet& p): Packet(p.data()) {}
				};

				struct cmu2_status: Packet
				{
					static const uint32_t _id = 0x604;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu2_status(): Packet(0) {}
					cmu2_status(const uint64_t _i): Packet(_i) {}
					cmu2_status(const Packet& p): Packet(p.data()) {}
				};

				struct cmu2_volts0: Packet
				{
					static const uint32_t _id = 0x605;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu2_volts0(): Packet(0) {}
					cmu2_volts0(const uint64_t _i): Packet(_i) {}
					cmu2_volts0(const Packet& p): Packet(p.data()) {}
				};

				struct cmu2_volts1: Packet
				{
					static const uint32_t _id = 0x606;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu2_volts1(): Packet(0) {}
					cmu2_volts1(const uint64_t _i): Packet(_i) {}
					cmu2_volts1(const Packet& p): Packet(p.data()) {}
				};

				struct cmu3_status: Packet
				{
					static const uint32_t _id = 0x607;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu3_status(): Packet(0) {}
					cmu3_status(const uint64_t _i): Packet(_i) {}
					cmu3_status(const Packet& p): Packet(p.data()) {}
				};

				struct cmu3_volts0: Packet
				{
					static const uint32_t _id = 0x608;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu3_volts0(): Packet(0) {}
					cmu3_volts0(const uint64_t _i): Packet(_i) {}
					cmu3_volts0(const Packet& p): Packet(p.data()) {}
				};

				struct cmu3_volts1: Packet
				{
					static const uint32_t _id = 0x609;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu3_volts1(): Packet(0) {}
					cmu3_volts1(const uint64_t _i): Packet(_i) {}
					cmu3_volts1(const Packet& p): Packet(p.data()) {}
				};

				struct cmu4_status: Packet
				{
					static const uint32_t _id = 0x60a;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu4_status(): Packet(0) {}
					cmu4_status(const uint64_t _i): Packet(_i) {}
					cmu4_status(const Packet& p): Packet(p.data()) {}
				};

				struct cmu4_volts0: Packet
				{
					static const uint32_t _id = 0x60b;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu4_volts0(): Packet(0) {}
					cmu4_volts0(const uint64_t _i): Packet(_i) {}
					cmu4_volts0(const Packet& p): Packet(p.data()) {}
				};

				struct cmu4_volts1: Packet
				{
					static const uint32_t _id = 0x60c;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					cmu4_volts1(): Packet(0) {}
					cmu4_volts1(const uint64_t _i): Packet(_i) {}
					cmu4_volts1(const Packet& p): Packet(p.data()) {}
				};
			}

			namespace bms1
			{
				struct reserved0: Packet
				{
					static const uint32_t _id = 0x6f0;
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
					reserved0(): Packet(0) {}
					reserved0(const uint64_t _i): Packet(_i) {}
					reserved0(const Packet& p): Packet(p.data()) {}
				};

				struct reserved1: Packet
				{
					static const uint32_t _id = 0x6f1;
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
					reserved1(): Packet(0) {}
					reserved1(const uint64_t _i): Packet(_i) {}
					reserved1(const Packet& p): Packet(p.data()) {}
				};

				struct reserved2: Packet
				{
					static const uint32_t _id = 0x6f2;
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
					reserved2(): Packet(0) {}
					reserved2(const uint64_t _i): Packet(_i) {}
					reserved2(const Packet& p): Packet(p.data()) {}
				};

				struct reserved3: Packet
				{
					static const uint32_t _id = 0x6f3;
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
					reserved3(): Packet(0) {}
					reserved3(const uint64_t _i): Packet(_i) {}
					reserved3(const Packet& p): Packet(p.data()) {}
				};

				struct pack_soc: Packet
				{
					static const uint32_t _id = 0x6f4;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single soc_Ah;
							Single soc_percentage;
						};
					};
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_soc(): Packet(0) {}
					pack_soc(const uint64_t _i): Packet(_i) {}
					pack_soc(const Packet& p): Packet(p.data()) {}
				};

				struct pack_bal_soc: Packet
				{
					static const uint32_t _id = 0x6f5;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Single soc_Ah;
							Single soc_percentage;
						};
					};
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_bal_soc(): Packet(0) {}
					pack_bal_soc(const uint64_t _i): Packet(_i) {}
					pack_bal_soc(const Packet& p): Packet(p.data()) {}
				};

				struct charger_cmd: Packet
				{
					static const uint32_t _id = 0x6f6;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					charger_cmd(): Packet(0) {}
					charger_cmd(const uint64_t _i): Packet(_i) {}
					charger_cmd(const Packet& p): Packet(p.data()) {}
				};

				struct precharge: Packet
				{
					static const uint32_t _id = 0x6f7;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt16 precharge_flags;
							UInt16 unused0;
							UInt16 unused1;
							UInt16 precharge_timer_flags;
						};
					};
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					precharge(): Packet(0) {}
					precharge(const uint64_t _i): Packet(_i) {}
					precharge(const Packet& p): Packet(p.data()) {}
				};

				struct max_min_volts: Packet
				{
					static const uint32_t _id = 0x6f8;
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
					max_min_volts(): Packet(0) {}
					max_min_volts(const uint64_t _i): Packet(_i) {}
					max_min_volts(const Packet& p): Packet(p.data()) {}
				};

				struct max_min_temps: Packet
				{
					static const uint32_t _id = 0x6f9;
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
					max_min_temps(): Packet(0) {}
					max_min_temps(const uint64_t _i): Packet(_i) {}
					max_min_temps(const Packet& p): Packet(p.data()) {}
				};

				struct pack_volt_curr: Packet
				{
					static const uint32_t _id = 0x6fa;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							Int32 voltage;
							Int32 current;
						};
					};
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_volt_curr(): Packet(0) {}
					pack_volt_curr(const uint64_t _i): Packet(_i) {}
					pack_volt_curr(const Packet& p): Packet(p.data()) {}
				};

				struct pack_status: Packet
				{
					static const uint32_t _id = 0x6fb;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pack_status(): Packet(0) {}
					pack_status(const uint64_t _i): Packet(_i) {}
					pack_status(const Packet& p): Packet(p.data()) {}
				};

				struct fan_status: Packet
				{
					static const uint32_t _id = 0x6fc;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					fan_status(): Packet(0) {}
					fan_status(const uint64_t _i): Packet(_i) {}
					fan_status(const Packet& p): Packet(p.data()) {}
				};

				struct extended_status: Packet
				{
					static const uint32_t _id = 0x6fd;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					extended_status(): Packet(0) {}
					extended_status(const uint64_t _i): Packet(_i) {}
					extended_status(const Packet& p): Packet(p.data()) {}
				};
			}

			namespace ws20
			{
				struct motor_id: Packet
				{
					static const uint32_t _id = 0x400;
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

				struct motor_status_info: Packet
				{
					static const uint32_t _id = 0x401;
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

				struct motor_bus: Packet
				{
					static const uint32_t _id = 0x402;
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

				struct motor_velocity: Packet
				{
					static const uint32_t _id = 0x403;
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

				struct motor_phase: Packet
				{
					static const uint32_t _id = 0x404;
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

				struct voltage_vector: Packet
				{
					static const uint32_t _id = 0x405;
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

				struct current_vector: Packet
				{
					static const uint32_t _id = 0x406;
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

				struct backemf: Packet
				{
					static const uint32_t _id = 0x407;
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

				struct rail_15v_1pt65v: Packet
				{
					static const uint32_t _id = 0x408;
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

				struct rail_2pt5v_1pt2v: Packet
				{
					static const uint32_t _id = 0x409;
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

				struct fanspeed: Packet
				{
					static const uint32_t _id = 0x40a;
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

				struct sinks_temp: Packet
				{
					static const uint32_t _id = 0x40b;
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

				struct cpu_airin_temp: Packet
				{
					static const uint32_t _id = 0x40c;
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

				struct cap_airout_temp: Packet
				{
					static const uint32_t _id = 0x40d;
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

				struct odom_bus_ah: Packet
				{
					static const uint32_t _id = 0x40e;
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

			namespace dc
			{
				struct driver_controls_id: Packet
				{
					static const uint32_t _id = 0x500;
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

				struct drive_cmd: Packet
				{
					static const uint32_t _id = 0x501;
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

				struct power_cmd: Packet
				{
					static const uint32_t _id = 0x502;
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

				struct reset_cmd: Packet
				{
					static const uint32_t _id = 0x503;
					union layout_t
					{
						uint64_t data;
						uint8_t bytes[8];
						struct PACKED
						{
							UInt32 unused0;
							UInt32 unused1;
						};
					};
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					reset_cmd(): Packet(0) {}
					reset_cmd(const uint64_t _i): Packet(_i) {}
					reset_cmd(const Packet& p): Packet(p.data()) {}
				};

				struct unused0: Packet
				{
					static const uint32_t _id = 0x504;
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
					unused0(): Packet(0) {}
					unused0(const uint64_t _i): Packet(_i) {}
					unused0(const Packet& p): Packet(p.data()) {}
				};

				struct switches: Packet
				{
					static const uint32_t _id = 0x505;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					switches(): Packet(0) {}
					switches(const uint64_t _i): Packet(_i) {}
					switches(const Packet& p): Packet(p.data()) {}
				};

				struct pedals: Packet
				{
					static const uint32_t _id = 0x506;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					pedals(): Packet(0) {}
					pedals(const uint64_t _i): Packet(_i) {}
					pedals(const Packet& p): Packet(p.data()) {}
				};
			}

			namespace os
			{
				struct user_cmds: Packet
				{
					static const uint32_t _id = 0x310;
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

				struct cruise_cmd: Packet
				{
					static const uint32_t _id = 0x311;
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
					cruise_cmd(): Packet(0) {}
					cruise_cmd(const uint64_t _i): Packet(_i) {}
					cruise_cmd(const Packet& p): Packet(p.data()) {}
				};
			}

			namespace mppt_master
			{
				struct unused0: Packet
				{
					static const uint32_t _id = 0x710;
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
					unused0(): Packet(0) {}
					unused0(const uint64_t _i): Packet(_i) {}
					unused0(const Packet& p): Packet(p.data()) {}
				};

				struct mppt1: Packet
				{
					static const uint32_t _id = 0x711;
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
					mppt1(): Packet(0) {}
					mppt1(const uint64_t _i): Packet(_i) {}
					mppt1(const Packet& p): Packet(p.data()) {}
				};

				struct mppt2: Packet
				{
					static const uint32_t _id = 0x712;
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
					mppt2(): Packet(0) {}
					mppt2(const uint64_t _i): Packet(_i) {}
					mppt2(const Packet& p): Packet(p.data()) {}
				};

				struct mppt3: Packet
				{
					static const uint32_t _id = 0x713;
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
					mppt3(): Packet(0) {}
					mppt3(const uint64_t _i): Packet(_i) {}
					mppt3(const Packet& p): Packet(p.data()) {}
				};
			}

			namespace mppt
			{
				struct unused0: Packet
				{
					static const uint32_t _id = 0x770;
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
					unused0(): Packet(0) {}
					unused0(const uint64_t _i): Packet(_i) {}
					unused0(const Packet& p): Packet(p.data()) {}
				};

				struct mppt1: Packet
				{
					static const uint32_t _id = 0x771;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt1(): Packet(0) {}
					mppt1(const uint64_t _i): Packet(_i) {}
					mppt1(const Packet& p): Packet(p.data()) {}
				};

				struct mppt2: Packet
				{
					static const uint32_t _id = 0x772;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt2(): Packet(0) {}
					mppt2(const uint64_t _i): Packet(_i) {}
					mppt2(const Packet& p): Packet(p.data()) {}
				};

				struct mppt3: Packet
				{
					static const uint32_t _id = 0x773;
					union layout_t
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
					uint32_t id() const {return _id;}
					layout_t &frame() {return *(layout_t *)&Packet::frame();}
					layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
					mppt3(): Packet(0) {}
					mppt3(const uint64_t _i): Packet(_i) {}
					mppt3(const Packet& p): Packet(p.data()) {}
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



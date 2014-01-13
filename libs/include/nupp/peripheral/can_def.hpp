/*
 * CAN frames are implemented as PACKED structs,
 * within namespaces for scoping.
 *
 * CAN addresses are uint16_t's,
 * within enum classes for scoping.
 */

#pragma GCC diagnostic ignored "-pedantic"

namespace nu
{
	namespace can
	{
		namespace frame
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

			namespace bms
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
								int32_t trip_code;
								uint32_t module;
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
						error_k = 0x211,
						uptime_k = 0x212,
						last_reset_k = 0x213,
						batt_bypass_k = 0x214,
						current_k = 0x215,
						cc_array_k = 0x216,
						cc_batt_k = 0x217,
						cc_mppt1_k = 0x218,
						cc_mppt2_k = 0x219,
						cc_mppt3_k = 0x21a,
						Wh_batt_k = 0x21b,
						Wh_mppt1_k = 0x21c,
						Wh_mppt2_k = 0x21d,
						Wh_mppt3_k = 0x21e,
						voltage_k = 0x21f,
						owVoltage_k = 0x220,
						temp_k = 0x221,
						trip_k = 0x222,
						last_trip_k = 0x223,
						trip_pt_current_k = 0x224,
						trip_pt_voltage_k = 0x225,
						trip_pt_temp_k = 0x226
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
								char bms_str[4];
								uint32_t reserved;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						heartbeat(): Packet(0) {}
						heartbeat(const uint64_t _i): Packet(_i) {}
						heartbeat(const Packet& p): Packet(p.data()) {}
					};

					class error: public Packet
					{
						static const uint32_t _id = 0x211;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								char message[8];
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						error(): Packet(0) {}
						error(const uint64_t _i): Packet(_i) {}
						error(const Packet& p): Packet(p.data()) {}
					};

					class uptime: public Packet
					{
						static const uint32_t _id = 0x212;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double seconds;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						uptime(): Packet(0) {}
						uptime(const uint64_t _i): Packet(_i) {}
						uptime(const Packet& p): Packet(p.data()) {}
					};

					class last_reset: public Packet
					{
						static const uint32_t _id = 0x213;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								int32_t last_reset_code;
								uint32_t reserved;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						last_reset(): Packet(0) {}
						last_reset(const uint64_t _i): Packet(_i) {}
						last_reset(const Packet& p): Packet(p.data()) {}
					};

					class batt_bypass: public Packet
					{
						static const uint32_t _id = 0x214;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								uint32_t module;
								float reserved;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						batt_bypass(): Packet(0) {}
						batt_bypass(const uint64_t _i): Packet(_i) {}
						batt_bypass(const Packet& p): Packet(p.data()) {}
					};

					class current: public Packet
					{
						static const uint32_t _id = 0x215;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								float array;
								float battery;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						current(): Packet(0) {}
						current(const uint64_t _i): Packet(_i) {}
						current(const Packet& p): Packet(p.data()) {}
					};

					class cc_array: public Packet
					{
						static const uint32_t _id = 0x216;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x217;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x218;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x219;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x21a;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x21b;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x21c;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x21d;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
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
						static const uint32_t _id = 0x21e;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								double count;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						Wh_mppt3(): Packet(0) {}
						Wh_mppt3(const uint64_t _i): Packet(_i) {}
						Wh_mppt3(const Packet& p): Packet(p.data()) {}
					};

					class voltage: public Packet
					{
						static const uint32_t _id = 0x21f;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								uint32_t module;
								float voltage;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						voltage(): Packet(0) {}
						voltage(const uint64_t _i): Packet(_i) {}
						voltage(const Packet& p): Packet(p.data()) {}
					};

					class owVoltage: public Packet
					{
						static const uint32_t _id = 0x220;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								uint32_t module;
								float ow_voltage;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						owVoltage(): Packet(0) {}
						owVoltage(const uint64_t _i): Packet(_i) {}
						owVoltage(const Packet& p): Packet(p.data()) {}
					};

					class temp: public Packet
					{
						static const uint32_t _id = 0x221;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								uint32_t sensor;
								float temp;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						temp(): Packet(0) {}
						temp(const uint64_t _i): Packet(_i) {}
						temp(const Packet& p): Packet(p.data()) {}
					};

					class trip: public Packet
					{
						static const uint32_t _id = 0x222;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								int32_t trip_code;
								uint32_t module;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						trip(): Packet(0) {}
						trip(const uint64_t _i): Packet(_i) {}
						trip(const Packet& p): Packet(p.data()) {}
					};

					class last_trip: public Packet
					{
						static const uint32_t _id = 0x223;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								int32_t trip_code;
								uint32_t module;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						last_trip(): Packet(0) {}
						last_trip(const uint64_t _i): Packet(_i) {}
						last_trip(const Packet& p): Packet(p.data()) {}
					};

					class trip_pt_current: public Packet
					{
						static const uint32_t _id = 0x224;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								float low;
								float high;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						trip_pt_current(): Packet(0) {}
						trip_pt_current(const uint64_t _i): Packet(_i) {}
						trip_pt_current(const Packet& p): Packet(p.data()) {}
					};

					class trip_pt_voltage: public Packet
					{
						static const uint32_t _id = 0x225;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								float low;
								float high;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						trip_pt_voltage(): Packet(0) {}
						trip_pt_voltage(const uint64_t _i): Packet(_i) {}
						trip_pt_voltage(const Packet& p): Packet(p.data()) {}
					};

					class trip_pt_temp: public Packet
					{
						static const uint32_t _id = 0x226;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								float low;
								float high;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						trip_pt_temp(): Packet(0) {}
						trip_pt_temp(const uint64_t _i): Packet(_i) {}
						trip_pt_temp(const Packet& p): Packet(p.data()) {}
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
								char drvId[4];
								uint32_t serialNo;
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
								float motorVelocity;
								float motorCurrent;
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
								float reserved;
								float busCurrent;
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
								uint32_t unused1;
								uint32_t unused2;
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
								char tritiumId[4];
								uint32_t serialNo;
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
								uint16_t limitFlags;
								uint16_t errorFlags;
								uint16_t activeMotor;
								uint16_t reserved;
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
								float busVoltage;
								float busCurrent;
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
								float motorVelocity;
								float vehicleVelocity;
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
								float phaseBCurrent;
								float phaseACurrent;
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
								float voltageIm;
								float voltageRe;
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
								float currentIm;
								float currentRe;
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
								float backEmfIm;
								float backEmfRe;
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
								float onePtSixtyFiveVRef;
								float fifteenVPowerRail;
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
								float onePtTwoVSupply;
								float twoPtFiveVSupply;
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
								float fanDrive;
								float fanRpm;
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
								float motorTemp;
								float heatsinkTemp;
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
								float processorTemp;
								float airInletTemp;
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
								float capacitorTemp;
								float airOutTemp;
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
								float odom;
								float dcBusAmpHours;
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
								float velocity;
								float current;
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
								uint16_t accel_pedal;
								uint16_t brake_pedal;
								uint16_t reserved;
								uint16_t reserved1;
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
						driver_input_k = 0x310
					};

					class driver_input: public Packet
					{
						static const uint32_t _id = 0x310;
					public:
						union layout_t
						{
							uint64_t data;
							uint8_t bytes[8];
							struct PACKED
							{
								uint16_t power;
								uint16_t gearFlags;
								uint16_t lightsFlags;
								uint16_t horn;
							};
						};
						uint32_t id() const {return _id;}
						layout_t &frame() {return *(layout_t *)&Packet::frame();}
						layout_t frame() const {frame_t f=Packet::frame(); return *(layout_t*)&f;}
						driver_input(): Packet(0) {}
						driver_input(const uint64_t _i): Packet(_i) {}
						driver_input(const Packet& p): Packet(p.data()) {}
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

#pragma GCC diagnostic warning "-pedantic"
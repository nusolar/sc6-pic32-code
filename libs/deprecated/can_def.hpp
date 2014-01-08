
	/**
	 * CAN frames are implemented as PACKED structs,
	 * within namespaces for scoping.
	 *
	 * CAN addresses are uint16_t's,
	 * within enum classes for scoping.
	 */

 namespace nu {
 namespace can {
 namespace frame {

			class Packet {
				static const uint32_t _id = 0;
			public:
				union frame_t {
					uint64_t data;
					uint8_t bytes[8];
				} frame;
				virtual uint32_t id() const {return _id;}
				virtual uint64_t data() const {return frame.data;}
				virtual uint64_t &data() {return frame.data;}
				virtual uint8_t  *bytes() {return frame.bytes;}
				virtual const uint8_t *bytes() const {return frame.bytes;}
				virtual ~Packet() {}
				ALWAYSINLINE Packet(): frame{0} {}
				ALWAYSINLINE Packet(const uint64_t _i): frame{_i} {}
				ALWAYSINLINE Packet(const Packet& p): frame{p.data()} {}
				Packet& operator= (const uint64_t _i) {data() = _i; return *this;}
				Packet& operator= (const Packet& p) {data() = p.data(); return *this;}
			};
 namespace bms {
 namespace rx {

		class trip: public Packet {
			static const uint32_t _id = 0x200;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					int32_t trip_code; uint32_t module;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE trip(): frame{0} {}
			ALWAYSINLINE trip(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_batt: public Packet {
			static const uint32_t _id = 0x201;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_batt(): frame{0} {}
			ALWAYSINLINE reset_cc_batt(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_batt(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_array: public Packet {
			static const uint32_t _id = 0x202;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_array(): frame{0} {}
			ALWAYSINLINE reset_cc_array(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_array(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_mppt1: public Packet {
			static const uint32_t _id = 0x203;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_mppt1(): frame{0} {}
			ALWAYSINLINE reset_cc_mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_mppt1(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_mppt2: public Packet {
			static const uint32_t _id = 0x204;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_mppt2(): frame{0} {}
			ALWAYSINLINE reset_cc_mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_mppt2(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_mppt3: public Packet {
			static const uint32_t _id = 0x205;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_mppt3(): frame{0} {}
			ALWAYSINLINE reset_cc_mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_mppt3(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_Wh: public Packet {
			static const uint32_t _id = 0x206;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_Wh(): frame{0} {}
			ALWAYSINLINE reset_cc_Wh(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_Wh(const Packet& p): frame{p.data()} {}
		};

		class reset_cc_all: public Packet {
			static const uint32_t _id = 0x207;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cc_all(): frame{0} {}
			ALWAYSINLINE reset_cc_all(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_all(const Packet& p): frame{p.data()} {}
		};

		class max: public Packet {
			static const uint32_t _id = 0x20f;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE max(): frame{0} {}
			ALWAYSINLINE max(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE max(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
trip_k = 0x200,
reset_cc_batt_k = 0x201,
reset_cc_array_k = 0x202,
reset_cc_mppt1_k = 0x203,
reset_cc_mppt2_k = 0x204,
reset_cc_mppt3_k = 0x205,
reset_cc_Wh_k = 0x206,
reset_cc_all_k = 0x207,
max_k = 0x20f};
}
 namespace tx {

		class heartbeat: public Packet {
			static const uint32_t _id = 0x210;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					char bms_str[4]; uint32_t reserved;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE heartbeat(): frame{0} {}
			ALWAYSINLINE heartbeat(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE heartbeat(const Packet& p): frame{p.data()} {}
		};

		class error: public Packet {
			static const uint32_t _id = 0x211;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					char msg[8];
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE error(): frame{0} {}
			ALWAYSINLINE error(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE error(const Packet& p): frame{p.data()} {}
		};

		class uptime: public Packet {
			static const uint32_t _id = 0x212;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double seconds;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE uptime(): frame{0} {}
			ALWAYSINLINE uptime(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE uptime(const Packet& p): frame{p.data()} {}
		};

		class last_reset: public Packet {
			static const uint32_t _id = 0x213;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					int32_t last_reset_code; uint32_t reserved;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE last_reset(): frame{0} {}
			ALWAYSINLINE last_reset(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE last_reset(const Packet& p): frame{p.data()} {}
		};

		class batt_bypass: public Packet {
			static const uint32_t _id = 0x214;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint32_t module; float reserved;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE batt_bypass(): frame{0} {}
			ALWAYSINLINE batt_bypass(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE batt_bypass(const Packet& p): frame{p.data()} {}
		};

		class current: public Packet {
			static const uint32_t _id = 0x215;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float array, battery;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE current(): frame{0} {}
			ALWAYSINLINE current(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE current(const Packet& p): frame{p.data()} {}
		};

		class cc_array: public Packet {
			static const uint32_t _id = 0x216;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cc_array(): frame{0} {}
			ALWAYSINLINE cc_array(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_array(const Packet& p): frame{p.data()} {}
		};

		class cc_batt: public Packet {
			static const uint32_t _id = 0x217;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cc_batt(): frame{0} {}
			ALWAYSINLINE cc_batt(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_batt(const Packet& p): frame{p.data()} {}
		};

		class cc_mppt1: public Packet {
			static const uint32_t _id = 0x218;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cc_mppt1(): frame{0} {}
			ALWAYSINLINE cc_mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_mppt1(const Packet& p): frame{p.data()} {}
		};

		class cc_mppt2: public Packet {
			static const uint32_t _id = 0x219;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cc_mppt2(): frame{0} {}
			ALWAYSINLINE cc_mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_mppt2(const Packet& p): frame{p.data()} {}
		};

		class cc_mppt3: public Packet {
			static const uint32_t _id = 0x21a;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cc_mppt3(): frame{0} {}
			ALWAYSINLINE cc_mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_mppt3(const Packet& p): frame{p.data()} {}
		};

		class Wh_batt: public Packet {
			static const uint32_t _id = 0x21b;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE Wh_batt(): frame{0} {}
			ALWAYSINLINE Wh_batt(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_batt(const Packet& p): frame{p.data()} {}
		};

		class Wh_mppt1: public Packet {
			static const uint32_t _id = 0x21c;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE Wh_mppt1(): frame{0} {}
			ALWAYSINLINE Wh_mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_mppt1(const Packet& p): frame{p.data()} {}
		};

		class Wh_mppt2: public Packet {
			static const uint32_t _id = 0x21d;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE Wh_mppt2(): frame{0} {}
			ALWAYSINLINE Wh_mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_mppt2(const Packet& p): frame{p.data()} {}
		};

		class Wh_mppt3: public Packet {
			static const uint32_t _id = 0x21e;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					double count;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE Wh_mppt3(): frame{0} {}
			ALWAYSINLINE Wh_mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_mppt3(const Packet& p): frame{p.data()} {}
		};

		class voltage: public Packet {
			static const uint32_t _id = 0x21f;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint32_t module; float voltage;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE voltage(): frame{0} {}
			ALWAYSINLINE voltage(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE voltage(const Packet& p): frame{p.data()} {}
		};

		class owVoltage: public Packet {
			static const uint32_t _id = 0x220;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint32_t module; float ow_voltage;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE owVoltage(): frame{0} {}
			ALWAYSINLINE owVoltage(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE owVoltage(const Packet& p): frame{p.data()} {}
		};

		class temp: public Packet {
			static const uint32_t _id = 0x221;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint32_t sensor; float temp;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE temp(): frame{0} {}
			ALWAYSINLINE temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE temp(const Packet& p): frame{p.data()} {}
		};

		class trip: public Packet {
			static const uint32_t _id = 0x222;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					int32_t trip_code; uint32_t module;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE trip(): frame{0} {}
			ALWAYSINLINE trip(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip(const Packet& p): frame{p.data()} {}
		};

		class last_trip: public Packet {
			static const uint32_t _id = 0x223;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					int32_t trip_code; uint32_t module;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE last_trip(): frame{0} {}
			ALWAYSINLINE last_trip(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE last_trip(const Packet& p): frame{p.data()} {}
		};

		class trip_pt_current: public Packet {
			static const uint32_t _id = 0x224;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float low, high;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE trip_pt_current(): frame{0} {}
			ALWAYSINLINE trip_pt_current(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip_pt_current(const Packet& p): frame{p.data()} {}
		};

		class trip_pt_voltage: public Packet {
			static const uint32_t _id = 0x225;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float low, high;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE trip_pt_voltage(): frame{0} {}
			ALWAYSINLINE trip_pt_voltage(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip_pt_voltage(const Packet& p): frame{p.data()} {}
		};

		class trip_pt_temp: public Packet {
			static const uint32_t _id = 0x226;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float low, high;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE trip_pt_temp(): frame{0} {}
			ALWAYSINLINE trip_pt_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip_pt_temp(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
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
trip_pt_temp_k = 0x226};
}
}
 namespace ws20 {
 namespace rx {

		class driver_controls_id: public Packet {
			static const uint32_t _id = 0x500;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					char drvId[4]; uint32_t serialNo;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE driver_controls_id(): frame{0} {}
			ALWAYSINLINE driver_controls_id(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE driver_controls_id(const Packet& p): frame{p.data()} {}
		};

		class drive_cmd: public Packet {
			static const uint32_t _id = 0x501;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float motorVelocity, motorCurrent;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE drive_cmd(): frame{0} {}
			ALWAYSINLINE drive_cmd(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE drive_cmd(const Packet& p): frame{p.data()} {}
		};

		class power_cmd: public Packet {
			static const uint32_t _id = 0x502;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float reserved, busCurrent;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE power_cmd(): frame{0} {}
			ALWAYSINLINE power_cmd(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE power_cmd(const Packet& p): frame{p.data()} {}
		};

		class reset_cmd: public Packet {
			static const uint32_t _id = 0x503;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint32_t unused1, unused2;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE reset_cmd(): frame{0} {}
			ALWAYSINLINE reset_cmd(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cmd(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
driver_controls_id_k = 0x500,
drive_cmd_k = 0x501,
power_cmd_k = 0x502,
reset_cmd_k = 0x503};
}
 namespace tx {

		class motor_id: public Packet {
			static const uint32_t _id = 0x400;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					char tritiumId[4]; uint32_t serialNo;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE motor_id(): frame{0} {}
			ALWAYSINLINE motor_id(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_id(const Packet& p): frame{p.data()} {}
		};

		class motor_status_info: public Packet {
			static const uint32_t _id = 0x401;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
	uint16_t limitFlags;
	uint16_t errorFlags;
	uint16_t activeMotor;
	uint16_t reserved __attribute__ ((__packed__));
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE motor_status_info(): frame{0} {}
			ALWAYSINLINE motor_status_info(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_status_info(const Packet& p): frame{p.data()} {}
		};

		class motor_bus: public Packet {
			static const uint32_t _id = 0x402;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float busVoltage, busCurrent;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE motor_bus(): frame{0} {}
			ALWAYSINLINE motor_bus(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_bus(const Packet& p): frame{p.data()} {}
		};

		class motor_velocity: public Packet {
			static const uint32_t _id = 0x403;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float motorVelocity, vehicleVelocity;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE motor_velocity(): frame{0} {}
			ALWAYSINLINE motor_velocity(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_velocity(const Packet& p): frame{p.data()} {}
		};

		class motor_phase: public Packet {
			static const uint32_t _id = 0x404;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float phaseBCurrent, phaseACurrent;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE motor_phase(): frame{0} {}
			ALWAYSINLINE motor_phase(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_phase(const Packet& p): frame{p.data()} {}
		};

		class voltage_vector: public Packet {
			static const uint32_t _id = 0x405;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float voltageIm, voltageRe;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE voltage_vector(): frame{0} {}
			ALWAYSINLINE voltage_vector(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE voltage_vector(const Packet& p): frame{p.data()} {}
		};

		class current_vector: public Packet {
			static const uint32_t _id = 0x406;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float currentIm, currentRe;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE current_vector(): frame{0} {}
			ALWAYSINLINE current_vector(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE current_vector(const Packet& p): frame{p.data()} {}
		};

		class backemf: public Packet {
			static const uint32_t _id = 0x407;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float backEmfIm, backEmfRe;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE backemf(): frame{0} {}
			ALWAYSINLINE backemf(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE backemf(const Packet& p): frame{p.data()} {}
		};

		class rail_15v_1pt65v: public Packet {
			static const uint32_t _id = 0x408;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float onePtSixtyFiveVRef, fifteenVPowerRail;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE rail_15v_1pt65v(): frame{0} {}
			ALWAYSINLINE rail_15v_1pt65v(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE rail_15v_1pt65v(const Packet& p): frame{p.data()} {}
		};

		class rail_2pt5v_1pt2v: public Packet {
			static const uint32_t _id = 0x409;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float onePtTwoVSupply, twoPtFiveVSupply;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE rail_2pt5v_1pt2v(): frame{0} {}
			ALWAYSINLINE rail_2pt5v_1pt2v(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE rail_2pt5v_1pt2v(const Packet& p): frame{p.data()} {}
		};

		class fanspeed: public Packet {
			static const uint32_t _id = 0x40a;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float fanDrive, fanRpm;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE fanspeed(): frame{0} {}
			ALWAYSINLINE fanspeed(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE fanspeed(const Packet& p): frame{p.data()} {}
		};

		class sinks_temp: public Packet {
			static const uint32_t _id = 0x40b;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float motorTemp, heatsinkTemp;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE sinks_temp(): frame{0} {}
			ALWAYSINLINE sinks_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE sinks_temp(const Packet& p): frame{p.data()} {}
		};

		class cpu_airin_temp: public Packet {
			static const uint32_t _id = 0x40c;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float processorTemp, airInletTemp;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cpu_airin_temp(): frame{0} {}
			ALWAYSINLINE cpu_airin_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cpu_airin_temp(const Packet& p): frame{p.data()} {}
		};

		class cap_airout_temp: public Packet {
			static const uint32_t _id = 0x40d;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float capacitorTemp, airOutTemp;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cap_airout_temp(): frame{0} {}
			ALWAYSINLINE cap_airout_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cap_airout_temp(const Packet& p): frame{p.data()} {}
		};

		class odom_bus_ah: public Packet {
			static const uint32_t _id = 0x40e;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float odom, dcBusAmpHours;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE odom_bus_ah(): frame{0} {}
			ALWAYSINLINE odom_bus_ah(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE odom_bus_ah(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
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
odom_bus_ah_k = 0x40e};
}
}
 namespace mppt {
 namespace rx {

		class mppt1: public Packet {
			static const uint32_t _id = 0x710;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE mppt1(): frame{0} {}
			ALWAYSINLINE mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt1(const Packet& p): frame{p.data()} {}
		};

		class mppt2: public Packet {
			static const uint32_t _id = 0x710;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE mppt2(): frame{0} {}
			ALWAYSINLINE mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt2(const Packet& p): frame{p.data()} {}
		};

		class mppt3: public Packet {
			static const uint32_t _id = 0x710;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE mppt3(): frame{0} {}
			ALWAYSINLINE mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt3(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
mppt1_k = 0x710,
mppt2_k = 0x710,
mppt3_k = 0x710};
}
 namespace tx {

		class mppt1: public Packet {
			static const uint32_t _id = 0x710;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE mppt1(): frame{0} {}
			ALWAYSINLINE mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt1(const Packet& p): frame{p.data()} {}
		};

		class mppt2: public Packet {
			static const uint32_t _id = 0x710;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE mppt2(): frame{0} {}
			ALWAYSINLINE mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt2(const Packet& p): frame{p.data()} {}
		};

		class mppt3: public Packet {
			static const uint32_t _id = 0x710;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE mppt3(): frame{0} {}
			ALWAYSINLINE mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt3(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
mppt1_k = 0x710,
mppt2_k = 0x710,
mppt3_k = 0x710};
}
}
 namespace sw {
 namespace rx {

		class buttons: public Packet {
			static const uint32_t _id = 0x300;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    hazard          :1;
	unsigned    horn            :1;
	unsigned    reserved        :21;
	unsigned    reserved1       :32;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE buttons(): frame{0} {}
			ALWAYSINLINE buttons(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE buttons(const Packet& p): frame{p.data()} {}
		};

		class lights: public Packet {
			static const uint32_t _id = 0x301;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    hazard          :1;
	unsigned    horn            :1;
	unsigned    radio           :1;
	unsigned    reserved        :20;
	unsigned    reserved1		:32;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE lights(): frame{0} {}
			ALWAYSINLINE lights(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE lights(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
buttons_k = 0x300,
lights_k = 0x301};
}
 namespace tx {

		class heartbeat: public Packet {
			static const uint32_t _id = 0x310;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					char dcStr[4]; uint32_t reserved;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE heartbeat(): frame{0} {}
			ALWAYSINLINE heartbeat(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE heartbeat(const Packet& p): frame{p.data()} {}
		};

		class error: public Packet {
			static const uint32_t _id = 0x311;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					char msg[8];
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE error(): frame{0} {}
			ALWAYSINLINE error(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE error(const Packet& p): frame{p.data()} {}
		};

		class buttons: public Packet {
			static const uint32_t _id = 0x312;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    hazard          :1;
	unsigned    horn            :1;
	unsigned    reserved        :21;
	unsigned    reserved1       :32;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE buttons(): frame{0} {}
			ALWAYSINLINE buttons(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE buttons(const Packet& p): frame{p.data()} {}
		};

		class lights: public Packet {
			static const uint32_t _id = 0x313;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    hazard          :1;
	unsigned    horn            :1;
	unsigned    radio           :1;
	unsigned    reserved        :20;
	unsigned    reserved1		:32;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE lights(): frame{0} {}
			ALWAYSINLINE lights(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE lights(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
heartbeat_k = 0x310,
error_k = 0x311,
buttons_k = 0x312,
lights_k = 0x313};
}
}
 namespace dc {
 namespace rx {

		class horn: public Packet {
			static const uint32_t _id = 0x110;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint64_t enabled;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE horn(): frame{0} {}
			ALWAYSINLINE horn(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE horn(const Packet& p): frame{p.data()} {}
		};

		class signals: public Packet {
			static const uint32_t _id = 0x111;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint32_t l, r;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE signals(): frame{0} {}
			ALWAYSINLINE signals(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE signals(const Packet& p): frame{p.data()} {}
		};

		class cruise: public Packet {
			static const uint32_t _id = 0x112;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					uint64_t enabled;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cruise(): frame{0} {}
			ALWAYSINLINE cruise(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cruise(const Packet& p): frame{p.data()} {}
		};

		class cruise_velocity_current: public Packet {
			static const uint32_t _id = 0x113;
		public:
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED {
					float velocity, current;
				} contents;
			} frame;
			ALWAYSINLINE uint32_t id() const {return _id;}
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			ALWAYSINLINE cruise_velocity_current(): frame{0} {}
			ALWAYSINLINE cruise_velocity_current(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cruise_velocity_current(const Packet& p): frame{p.data()} {}
		};
 enum addrs {
horn_k = 0x110,
signals_k = 0x111,
cruise_k = 0x112,
cruise_velocity_current_k = 0x113};
}
 namespace tx {
	 class pedals: public Packet {
		static const uint32_t _id = 0x120;
	public:
		union frame_t {
			uint64_t data;
			uint8_t bytes[8];
			struct PACKED {
				unsigned accel_pedal :10;
				unsigned brake_en	:1;
				unsigned reserved0	:21;
				unsigned reserved1	:32;
			} contents;
		} frame;
		ALWAYSINLINE uint32_t id() const {return _id;}
		ALWAYSINLINE uint64_t data() const {return frame.data;}
		ALWAYSINLINE uint64_t &data() {return frame.data;}
		ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
		ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
		ALWAYSINLINE pedals(): frame{0} {}
		ALWAYSINLINE pedals(const uint64_t _i): frame{_i} {}
		ALWAYSINLINE pedals(const Packet& p): frame{p.data()} {}
	 };
 enum addrs {
	 pedals_k = 0x120
};
}
}

 namespace os {
	 namespace tx {
		 class driver_input: public Packet {
			 static const uint32_t _id = 0x310;
		 public:
			 union frame_t {
				 uint64_t data;
				 uint8_t bytes[8];
				 struct PACKED {
					 unsigned	power	:1;
					 unsigned	drive	:1;
					 unsigned	reverse	:1;
					 unsigned	signals	:2;
					 unsigned	headlights	:1;
					 unsigned	horn	:1;
					 unsigned	reserved0	:25;
					 unsigned	reserved1	:32;
				 } contents;
			 } frame;
			 ALWAYSINLINE uint32_t id() const {return _id;}
			 ALWAYSINLINE uint64_t data() const {return frame.data;}
			 ALWAYSINLINE uint64_t &data() {return frame.data;}
			 ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			 ALWAYSINLINE const uint8_t *bytes() const {return frame.bytes;}
			 ALWAYSINLINE driver_input(): frame{0} {}
			 ALWAYSINLINE driver_input(const uint64_t _i): frame{_i} {}
			 ALWAYSINLINE driver_input(const Packet& p): frame{p.data()} {}
		 };
		 enum addrs {
			driver_input_k = 0x310
		 };
	 }
 }
}
}
}

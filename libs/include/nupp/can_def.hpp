
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

	struct Packet {
				union frame_t {
					uint64_t data;
					uint8_t bytes[8];
				} frame;
				virtual ~Packet() {}
				virtual uint64_t data() const {return frame.data;}
				virtual uint64_t &data() {return frame.data;}
				virtual uint8_t  *bytes() {return frame.bytes;}
				ALWAYSINLINE Packet(): frame{0} {}
				ALWAYSINLINE Packet(const uint64_t _i): frame{_i} {}
				ALWAYSINLINE Packet(const Packet& p): frame{p.data()} {}
				Packet& operator= (const uint64_t _i) {data() = _i; return *this;}
				Packet& operator= (const Packet& p) {data() = p.data(); return *this;}
			};
 namespace bms {
 namespace rx {

		struct trip: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t trip_code; uint32_t module;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE trip(): frame{0} {}
			ALWAYSINLINE trip(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_batt: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_batt(): frame{0} {}
			ALWAYSINLINE reset_cc_batt(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_batt(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_array: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_array(): frame{0} {}
			ALWAYSINLINE reset_cc_array(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_array(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_mppt1(): frame{0} {}
			ALWAYSINLINE reset_cc_mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_mppt1(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_mppt2(): frame{0} {}
			ALWAYSINLINE reset_cc_mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_mppt2(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_mppt3(): frame{0} {}
			ALWAYSINLINE reset_cc_mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_mppt3(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_Wh: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_Wh(): frame{0} {}
			ALWAYSINLINE reset_cc_Wh(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_Wh(const Packet& p): frame{p.data()} {}
		};

		struct reset_cc_all: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cc_all(): frame{0} {}
			ALWAYSINLINE reset_cc_all(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cc_all(const Packet& p): frame{p.data()} {}
		};

		struct max: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE max(): frame{0} {}
			ALWAYSINLINE max(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE max(const Packet& p): frame{p.data()} {}
		};
}
 namespace tx {

		struct heartbeat: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char bms_str[4]; uint32_t reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE heartbeat(): frame{0} {}
			ALWAYSINLINE heartbeat(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE heartbeat(const Packet& p): frame{p.data()} {}
		};

		struct error: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char msg[8];
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE error(): frame{0} {}
			ALWAYSINLINE error(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE error(const Packet& p): frame{p.data()} {}
		};

		struct uptime: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double seconds;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE uptime(): frame{0} {}
			ALWAYSINLINE uptime(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE uptime(const Packet& p): frame{p.data()} {}
		};

		struct last_reset: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t last_reset_code; uint32_t reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE last_reset(): frame{0} {}
			ALWAYSINLINE last_reset(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE last_reset(const Packet& p): frame{p.data()} {}
		};

		struct batt_bypass: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t module; float reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE batt_bypass(): frame{0} {}
			ALWAYSINLINE batt_bypass(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE batt_bypass(const Packet& p): frame{p.data()} {}
		};

		struct current: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float array, battery;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE current(): frame{0} {}
			ALWAYSINLINE current(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE current(const Packet& p): frame{p.data()} {}
		};

		struct cc_array: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cc_array(): frame{0} {}
			ALWAYSINLINE cc_array(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_array(const Packet& p): frame{p.data()} {}
		};

		struct cc_batt: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cc_batt(): frame{0} {}
			ALWAYSINLINE cc_batt(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_batt(const Packet& p): frame{p.data()} {}
		};

		struct cc_mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cc_mppt1(): frame{0} {}
			ALWAYSINLINE cc_mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_mppt1(const Packet& p): frame{p.data()} {}
		};

		struct cc_mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cc_mppt2(): frame{0} {}
			ALWAYSINLINE cc_mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_mppt2(const Packet& p): frame{p.data()} {}
		};

		struct cc_mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cc_mppt3(): frame{0} {}
			ALWAYSINLINE cc_mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cc_mppt3(const Packet& p): frame{p.data()} {}
		};

		struct Wh_batt: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE Wh_batt(): frame{0} {}
			ALWAYSINLINE Wh_batt(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_batt(const Packet& p): frame{p.data()} {}
		};

		struct Wh_mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE Wh_mppt1(): frame{0} {}
			ALWAYSINLINE Wh_mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_mppt1(const Packet& p): frame{p.data()} {}
		};

		struct Wh_mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE Wh_mppt2(): frame{0} {}
			ALWAYSINLINE Wh_mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_mppt2(const Packet& p): frame{p.data()} {}
		};

		struct Wh_mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE Wh_mppt3(): frame{0} {}
			ALWAYSINLINE Wh_mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE Wh_mppt3(const Packet& p): frame{p.data()} {}
		};

		struct voltage: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t module; float voltage;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE voltage(): frame{0} {}
			ALWAYSINLINE voltage(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE voltage(const Packet& p): frame{p.data()} {}
		};

		struct owVoltage: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t module; float ow_voltage;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE owVoltage(): frame{0} {}
			ALWAYSINLINE owVoltage(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE owVoltage(const Packet& p): frame{p.data()} {}
		};

		struct temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t sensor; float temp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE temp(): frame{0} {}
			ALWAYSINLINE temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE temp(const Packet& p): frame{p.data()} {}
		};

		struct trip: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t trip_code; uint32_t module;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE trip(): frame{0} {}
			ALWAYSINLINE trip(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip(const Packet& p): frame{p.data()} {}
		};

		struct last_trip: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t trip_code; uint32_t module;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE last_trip(): frame{0} {}
			ALWAYSINLINE last_trip(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE last_trip(const Packet& p): frame{p.data()} {}
		};

		struct trip_pt_current: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float low, high;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE trip_pt_current(): frame{0} {}
			ALWAYSINLINE trip_pt_current(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip_pt_current(const Packet& p): frame{p.data()} {}
		};

		struct trip_pt_voltage: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float low, high;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE trip_pt_voltage(): frame{0} {}
			ALWAYSINLINE trip_pt_voltage(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip_pt_voltage(const Packet& p): frame{p.data()} {}
		};

		struct trip_pt_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float low, high;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE trip_pt_temp(): frame{0} {}
			ALWAYSINLINE trip_pt_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE trip_pt_temp(const Packet& p): frame{p.data()} {}
		};
}
}
 namespace mppt {
 namespace rx {

		struct mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE mppt1(): frame{0} {}
			ALWAYSINLINE mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt1(const Packet& p): frame{p.data()} {}
		};

		struct mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE mppt2(): frame{0} {}
			ALWAYSINLINE mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt2(const Packet& p): frame{p.data()} {}
		};

		struct mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE mppt3(): frame{0} {}
			ALWAYSINLINE mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt3(const Packet& p): frame{p.data()} {}
		};
}
 namespace tx {

		struct mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE mppt1(): frame{0} {}
			ALWAYSINLINE mppt1(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt1(const Packet& p): frame{p.data()} {}
		};

		struct mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE mppt2(): frame{0} {}
			ALWAYSINLINE mppt2(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt2(const Packet& p): frame{p.data()} {}
		};

		struct mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE mppt3(): frame{0} {}
			ALWAYSINLINE mppt3(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE mppt3(const Packet& p): frame{p.data()} {}
		};
}
}
 namespace dc {
 namespace rx {

		struct horn: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint64_t enabled;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE horn(): frame{0} {}
			ALWAYSINLINE horn(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE horn(const Packet& p): frame{p.data()} {}
		};

		struct signals: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t l, r;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE signals(): frame{0} {}
			ALWAYSINLINE signals(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE signals(const Packet& p): frame{p.data()} {}
		};

		struct cruise: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint64_t enabled;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cruise(): frame{0} {}
			ALWAYSINLINE cruise(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cruise(const Packet& p): frame{p.data()} {}
		};

		struct cruise_velocity_current: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float velocity, current;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cruise_velocity_current(): frame{0} {}
			ALWAYSINLINE cruise_velocity_current(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cruise_velocity_current(const Packet& p): frame{p.data()} {}
		};
}
 namespace tx {
}
}
 namespace ws20 {
 namespace rx {

		struct driver_controls_id: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char drvId[4]; uint32_t serialNo;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE driver_controls_id(): frame{0} {}
			ALWAYSINLINE driver_controls_id(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE driver_controls_id(const Packet& p): frame{p.data()} {}
		};

		struct drive_cmd: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float motorVelocity, motorCurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE drive_cmd(): frame{0} {}
			ALWAYSINLINE drive_cmd(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE drive_cmd(const Packet& p): frame{p.data()} {}
		};

		struct power_cmd: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float reserved, busCurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE power_cmd(): frame{0} {}
			ALWAYSINLINE power_cmd(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE power_cmd(const Packet& p): frame{p.data()} {}
		};

		struct reset_cmd: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t unused1, unused2;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE reset_cmd(): frame{0} {}
			ALWAYSINLINE reset_cmd(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE reset_cmd(const Packet& p): frame{p.data()} {}
		};
}
 namespace tx {

		struct motor_id: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char tritiumId[4]; uint32_t serialNo;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE motor_id(): frame{0} {}
			ALWAYSINLINE motor_id(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_id(const Packet& p): frame{p.data()} {}
		};

		struct motor_status_info: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
	uint16_t limitFlags;
	uint16_t errorFlags;
	uint16_t activeMotor;
	uint16_t reserved __attribute__ ((__packed__));
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE motor_status_info(): frame{0} {}
			ALWAYSINLINE motor_status_info(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_status_info(const Packet& p): frame{p.data()} {}
		};

		struct motor_bus: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float busVoltage, busCurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE motor_bus(): frame{0} {}
			ALWAYSINLINE motor_bus(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_bus(const Packet& p): frame{p.data()} {}
		};

		struct motor_velocity: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float motorVelocity, vehicleVelocity;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE motor_velocity(): frame{0} {}
			ALWAYSINLINE motor_velocity(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_velocity(const Packet& p): frame{p.data()} {}
		};

		struct motor_phase: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float phaseBCurrent, phaseACurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE motor_phase(): frame{0} {}
			ALWAYSINLINE motor_phase(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE motor_phase(const Packet& p): frame{p.data()} {}
		};

		struct voltage_vector: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float voltageIm, voltageRe;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE voltage_vector(): frame{0} {}
			ALWAYSINLINE voltage_vector(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE voltage_vector(const Packet& p): frame{p.data()} {}
		};

		struct current_vector: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float currentIm, currentRe;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE current_vector(): frame{0} {}
			ALWAYSINLINE current_vector(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE current_vector(const Packet& p): frame{p.data()} {}
		};

		struct backemf: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float backEmfIm, backEmfRe;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE backemf(): frame{0} {}
			ALWAYSINLINE backemf(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE backemf(const Packet& p): frame{p.data()} {}
		};

		struct rail_15v_1pt65v: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float onePtSixtyFiveVRef, fifteenVPowerRail;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE rail_15v_1pt65v(): frame{0} {}
			ALWAYSINLINE rail_15v_1pt65v(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE rail_15v_1pt65v(const Packet& p): frame{p.data()} {}
		};

		struct rail_2pt5v_1pt2v: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float onePtTwoVSupply, twoPtFiveVSupply;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE rail_2pt5v_1pt2v(): frame{0} {}
			ALWAYSINLINE rail_2pt5v_1pt2v(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE rail_2pt5v_1pt2v(const Packet& p): frame{p.data()} {}
		};

		struct fanspeed: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float fanDrive, fanRpm;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE fanspeed(): frame{0} {}
			ALWAYSINLINE fanspeed(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE fanspeed(const Packet& p): frame{p.data()} {}
		};

		struct sinks_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float motorTemp, heatsinkTemp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE sinks_temp(): frame{0} {}
			ALWAYSINLINE sinks_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE sinks_temp(const Packet& p): frame{p.data()} {}
		};

		struct cpu_airin_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float processorTemp, airInletTemp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cpu_airin_temp(): frame{0} {}
			ALWAYSINLINE cpu_airin_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cpu_airin_temp(const Packet& p): frame{p.data()} {}
		};

		struct cap_airout_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float capacitorTemp, airOutTemp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE cap_airout_temp(): frame{0} {}
			ALWAYSINLINE cap_airout_temp(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE cap_airout_temp(const Packet& p): frame{p.data()} {}
		};

		struct odom_bus_ah: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float odom, dcBusAmpHours;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE odom_bus_ah(): frame{0} {}
			ALWAYSINLINE odom_bus_ah(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE odom_bus_ah(const Packet& p): frame{p.data()} {}
		};
}
}
 namespace sw {
 namespace rx {

		struct buttons: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :25;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE buttons(): frame{0} {}
			ALWAYSINLINE buttons(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE buttons(const Packet& p): frame{p.data()} {}
		};

		struct lights: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    radio           :1;
	unsigned    yes             :1;
	unsigned    hazard          :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_up       :1;
	unsigned    maybe           :1;
	unsigned    no              :1;
	unsigned    horn            :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :20;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE lights(): frame{0} {}
			ALWAYSINLINE lights(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE lights(const Packet& p): frame{p.data()} {}
		};
}
 namespace tx {

		struct heartbeat: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char dcStr[4]; uint32_t reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE heartbeat(): frame{0} {}
			ALWAYSINLINE heartbeat(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE heartbeat(const Packet& p): frame{p.data()} {}
		};

		struct error: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char msg[8];
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE error(): frame{0} {}
			ALWAYSINLINE error(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE error(const Packet& p): frame{p.data()} {}
		};

		struct buttons: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :25;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE buttons(): frame{0} {}
			ALWAYSINLINE buttons(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE buttons(const Packet& p): frame{p.data()} {}
		};

		struct lights: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    radio           :1;
	unsigned    yes             :1;
	unsigned    hazard          :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_up       :1;
	unsigned    maybe           :1;
	unsigned    no              :1;
	unsigned    horn            :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :20;
				} s;
			} frame;
			ALWAYSINLINE uint64_t data() const {return frame.data;}
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			ALWAYSINLINE lights(): frame{0} {}
			ALWAYSINLINE lights(const uint64_t _i): frame{_i} {}
			ALWAYSINLINE lights(const Packet& p): frame{p.data()} {}
		};
}
}
}
 namespace addr {
 namespace bms {
 namespace rx {
 enum addrs {
trip_k = 512,
reset_cc_batt_k,
reset_cc_array_k,
reset_cc_mppt1_k,
reset_cc_mppt2_k,
reset_cc_mppt3_k,
reset_cc_Wh_k,
reset_cc_all_k,
max_k = 527};
}
 namespace tx {
 enum addrs {
heartbeat_k = 528,
error_k,
uptime_k,
last_reset_k,
batt_bypass_k,
current_k,
cc_array_k,
cc_batt_k,
cc_mppt1_k,
cc_mppt2_k,
cc_mppt3_k,
Wh_batt_k,
Wh_mppt1_k,
Wh_mppt2_k,
Wh_mppt3_k,
voltage_k,
owVoltage_k,
temp_k,
trip_k,
last_trip_k,
trip_pt_current_k,
trip_pt_voltage_k,
trip_pt_temp_k};
}
}
 namespace mppt {
 namespace rx {
 enum addrs {
mppt1_k = 1808,
mppt2_k = 1808,
mppt3_k = 1808};
}
 namespace tx {
 enum addrs {
mppt1_k = 1808,
mppt2_k = 1808,
mppt3_k = 1808};
}
}
 namespace dc {
 namespace rx {
 enum addrs {
horn_k = 272,
signals_k,
cruise_k,
cruise_velocity_current_k};
}
 namespace tx {
 enum addrs {
};
}
}
 namespace ws20 {
 namespace rx {
 enum addrs {
driver_controls_id_k = 1024,
drive_cmd_k,
power_cmd_k,
reset_cmd_k};
}
 namespace tx {
 enum addrs {
motor_id_k = 1024,
motor_status_info_k,
motor_bus_k,
motor_velocity_k,
motor_phase_k,
voltage_vector_k,
current_vector_k,
backemf_k,
rail_15v_1pt65v_k,
rail_2pt5v_1pt2v_k,
fanspeed_k,
sinks_temp_k,
cpu_airin_temp_k,
cap_airout_temp_k,
odom_bus_ah_k};
}
}
 namespace sw {
 namespace rx {
 enum addrs {
buttons_k = 768,
lights_k};
}
 namespace tx {
 enum addrs {
heartbeat_k = 784,
error_k,
buttons_k,
lights_k};
}
}
}
}
}

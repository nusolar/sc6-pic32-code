
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
				virtual uint64_t &data() {return frame.data;};
				virtual uint8_t  *bytes() {return frame.bytes;};
				Packet(): frame{0} {}
				Packet(uint64_t &_i): frame{_i} {}
				Packet(Packet& p): frame{p.data()} {}
				Packet& operator= (uint64_t &_i) {data() = _i; return *this;}
				Packet& operator= (Packet& p) {data() = p.data(); return *this;}
			};
 namespace sw {
 namespace rx {

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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			lights(): frame{0} {}
			lights(uint64_t &_i): frame{_i} {}
			lights(Packet& p): frame{p.data()} {}
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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			heartbeat(): frame{0} {}
			heartbeat(uint64_t &_i): frame{_i} {}
			heartbeat(Packet& p): frame{p.data()} {}
		};

		struct error: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char msg[8];
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			error(): frame{0} {}
			error(uint64_t &_i): frame{_i} {}
			error(Packet& p): frame{p.data()} {}
		};

		struct buttons: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					
	unsigned    left            :1;
	unsigned    right           :1;
	unsigned    yes             :1;
	unsigned    no              :1;
	unsigned    maybe           :1;
	unsigned    hazard          :1;
	unsigned    horn            :1;
	unsigned    cruise_en       :1;
	unsigned    cruise_mode     :1;
	unsigned    cruise_up       :1;
	unsigned    cruise_down     :1;
	unsigned    reserved        :21;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			buttons(): frame{0} {}
			buttons(uint64_t &_i): frame{_i} {}
			buttons(Packet& p): frame{p.data()} {}
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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			lights(): frame{0} {}
			lights(uint64_t &_i): frame{_i} {}
			lights(Packet& p): frame{p.data()} {}
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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			horn(): frame{0} {}
			horn(uint64_t &_i): frame{_i} {}
			horn(Packet& p): frame{p.data()} {}
		};

		struct signals: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t l, r;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			signals(): frame{0} {}
			signals(uint64_t &_i): frame{_i} {}
			signals(Packet& p): frame{p.data()} {}
		};

		struct cruise: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint64_t enabled;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cruise(): frame{0} {}
			cruise(uint64_t &_i): frame{_i} {}
			cruise(Packet& p): frame{p.data()} {}
		};

		struct cruise_velocity_current: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float velocity, current;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cruise_velocity_current(): frame{0} {}
			cruise_velocity_current(uint64_t &_i): frame{_i} {}
			cruise_velocity_current(Packet& p): frame{p.data()} {}
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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			driver_controls_id(): frame{0} {}
			driver_controls_id(uint64_t &_i): frame{_i} {}
			driver_controls_id(Packet& p): frame{p.data()} {}
		};

		struct drive_cmd: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float motorVelocity, motorCurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			drive_cmd(): frame{0} {}
			drive_cmd(uint64_t &_i): frame{_i} {}
			drive_cmd(Packet& p): frame{p.data()} {}
		};

		struct power_cmd: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float reserved, busCurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			power_cmd(): frame{0} {}
			power_cmd(uint64_t &_i): frame{_i} {}
			power_cmd(Packet& p): frame{p.data()} {}
		};

		struct reset_cmd: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t unused1, unused2;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cmd(): frame{0} {}
			reset_cmd(uint64_t &_i): frame{_i} {}
			reset_cmd(Packet& p): frame{p.data()} {}
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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			motor_id(): frame{0} {}
			motor_id(uint64_t &_i): frame{_i} {}
			motor_id(Packet& p): frame{p.data()} {}
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
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			motor_status_info(): frame{0} {}
			motor_status_info(uint64_t &_i): frame{_i} {}
			motor_status_info(Packet& p): frame{p.data()} {}
		};

		struct motor_bus: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float busVoltage, busCurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			motor_bus(): frame{0} {}
			motor_bus(uint64_t &_i): frame{_i} {}
			motor_bus(Packet& p): frame{p.data()} {}
		};

		struct motor_velocity: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float motorVelocity, vehicleVelocity;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			motor_velocity(): frame{0} {}
			motor_velocity(uint64_t &_i): frame{_i} {}
			motor_velocity(Packet& p): frame{p.data()} {}
		};

		struct motor_phase: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float phaseBCurrent, phaseACurrent;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			motor_phase(): frame{0} {}
			motor_phase(uint64_t &_i): frame{_i} {}
			motor_phase(Packet& p): frame{p.data()} {}
		};

		struct voltage_vector: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float voltageIm, voltageRe;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			voltage_vector(): frame{0} {}
			voltage_vector(uint64_t &_i): frame{_i} {}
			voltage_vector(Packet& p): frame{p.data()} {}
		};

		struct current_vector: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float currentIm, currentRe;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			current_vector(): frame{0} {}
			current_vector(uint64_t &_i): frame{_i} {}
			current_vector(Packet& p): frame{p.data()} {}
		};

		struct backemf: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float backEmfIm, backEmfRe;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			backemf(): frame{0} {}
			backemf(uint64_t &_i): frame{_i} {}
			backemf(Packet& p): frame{p.data()} {}
		};

		struct rail_15v_1pt65v: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float onePtSixtyFiveVRef, fifteenVPowerRail;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			rail_15v_1pt65v(): frame{0} {}
			rail_15v_1pt65v(uint64_t &_i): frame{_i} {}
			rail_15v_1pt65v(Packet& p): frame{p.data()} {}
		};

		struct rail_2pt5v_1pt2v: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float onePtTwoVSupply, twoPtFiveVSupply;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			rail_2pt5v_1pt2v(): frame{0} {}
			rail_2pt5v_1pt2v(uint64_t &_i): frame{_i} {}
			rail_2pt5v_1pt2v(Packet& p): frame{p.data()} {}
		};

		struct fanspeed: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float fanDrive, fanRpm;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			fanspeed(): frame{0} {}
			fanspeed(uint64_t &_i): frame{_i} {}
			fanspeed(Packet& p): frame{p.data()} {}
		};

		struct sinks_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float motorTemp, heatsinkTemp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			sinks_temp(): frame{0} {}
			sinks_temp(uint64_t &_i): frame{_i} {}
			sinks_temp(Packet& p): frame{p.data()} {}
		};

		struct cpu_airin_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float processorTemp, airInletTemp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cpu_airin_temp(): frame{0} {}
			cpu_airin_temp(uint64_t &_i): frame{_i} {}
			cpu_airin_temp(Packet& p): frame{p.data()} {}
		};

		struct cap_airout_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float capacitorTemp, airOutTemp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cap_airout_temp(): frame{0} {}
			cap_airout_temp(uint64_t &_i): frame{_i} {}
			cap_airout_temp(Packet& p): frame{p.data()} {}
		};

		struct odom_bus_ah: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float odom, dcBusAmpHours;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			odom_bus_ah(): frame{0} {}
			odom_bus_ah(uint64_t &_i): frame{_i} {}
			odom_bus_ah(Packet& p): frame{p.data()} {}
		};
}
}
 namespace bms {
 namespace rx {

		struct trip: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float trip_code, module;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			trip(): frame{0} {}
			trip(uint64_t &_i): frame{_i} {}
			trip(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_batt: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_batt(): frame{0} {}
			reset_cc_batt(uint64_t &_i): frame{_i} {}
			reset_cc_batt(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_array: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_array(): frame{0} {}
			reset_cc_array(uint64_t &_i): frame{_i} {}
			reset_cc_array(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_mppt1(): frame{0} {}
			reset_cc_mppt1(uint64_t &_i): frame{_i} {}
			reset_cc_mppt1(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_mppt2(): frame{0} {}
			reset_cc_mppt2(uint64_t &_i): frame{_i} {}
			reset_cc_mppt2(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_mppt3(): frame{0} {}
			reset_cc_mppt3(uint64_t &_i): frame{_i} {}
			reset_cc_mppt3(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_Wh: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_Wh(): frame{0} {}
			reset_cc_Wh(uint64_t &_i): frame{_i} {}
			reset_cc_Wh(Packet& p): frame{p.data()} {}
		};

		struct reset_cc_all: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			reset_cc_all(): frame{0} {}
			reset_cc_all(uint64_t &_i): frame{_i} {}
			reset_cc_all(Packet& p): frame{p.data()} {}
		};

		struct max: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			max(): frame{0} {}
			max(uint64_t &_i): frame{_i} {}
			max(Packet& p): frame{p.data()} {}
		};
}
 namespace tx {

		struct heartbeat: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char bmsStr[4]; uint32_t reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			heartbeat(): frame{0} {}
			heartbeat(uint64_t &_i): frame{_i} {}
			heartbeat(Packet& p): frame{p.data()} {}
		};

		struct error: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					char msg[8];
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			error(): frame{0} {}
			error(uint64_t &_i): frame{_i} {}
			error(Packet& p): frame{p.data()} {}
		};

		struct uptime: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double seconds;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			uptime(): frame{0} {}
			uptime(uint64_t &_i): frame{_i} {}
			uptime(Packet& p): frame{p.data()} {}
		};

		struct last_reset: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t lastResetCode; uint32_t reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			last_reset(): frame{0} {}
			last_reset(uint64_t &_i): frame{_i} {}
			last_reset(Packet& p): frame{p.data()} {}
		};

		struct batt_bypass: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t module; float reserved;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			batt_bypass(): frame{0} {}
			batt_bypass(uint64_t &_i): frame{_i} {}
			batt_bypass(Packet& p): frame{p.data()} {}
		};

		struct current: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float array, battery;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			current(): frame{0} {}
			current(uint64_t &_i): frame{_i} {}
			current(Packet& p): frame{p.data()} {}
		};

		struct cc_array: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cc_array(): frame{0} {}
			cc_array(uint64_t &_i): frame{_i} {}
			cc_array(Packet& p): frame{p.data()} {}
		};

		struct cc_batt: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cc_batt(): frame{0} {}
			cc_batt(uint64_t &_i): frame{_i} {}
			cc_batt(Packet& p): frame{p.data()} {}
		};

		struct cc_mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cc_mppt1(): frame{0} {}
			cc_mppt1(uint64_t &_i): frame{_i} {}
			cc_mppt1(Packet& p): frame{p.data()} {}
		};

		struct cc_mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cc_mppt2(): frame{0} {}
			cc_mppt2(uint64_t &_i): frame{_i} {}
			cc_mppt2(Packet& p): frame{p.data()} {}
		};

		struct cc_mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			cc_mppt3(): frame{0} {}
			cc_mppt3(uint64_t &_i): frame{_i} {}
			cc_mppt3(Packet& p): frame{p.data()} {}
		};

		struct Wh_batt: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			Wh_batt(): frame{0} {}
			Wh_batt(uint64_t &_i): frame{_i} {}
			Wh_batt(Packet& p): frame{p.data()} {}
		};

		struct Wh_mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			Wh_mppt1(): frame{0} {}
			Wh_mppt1(uint64_t &_i): frame{_i} {}
			Wh_mppt1(Packet& p): frame{p.data()} {}
		};

		struct Wh_mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			Wh_mppt2(): frame{0} {}
			Wh_mppt2(uint64_t &_i): frame{_i} {}
			Wh_mppt2(Packet& p): frame{p.data()} {}
		};

		struct Wh_mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					double count;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			Wh_mppt3(): frame{0} {}
			Wh_mppt3(uint64_t &_i): frame{_i} {}
			Wh_mppt3(Packet& p): frame{p.data()} {}
		};

		struct voltage: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t module; float voltage;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			voltage(): frame{0} {}
			voltage(uint64_t &_i): frame{_i} {}
			voltage(Packet& p): frame{p.data()} {}
		};

		struct owVoltage: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t module; float owVoltage;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			owVoltage(): frame{0} {}
			owVoltage(uint64_t &_i): frame{_i} {}
			owVoltage(Packet& p): frame{p.data()} {}
		};

		struct temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					uint32_t sensor; float temp;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			temp(): frame{0} {}
			temp(uint64_t &_i): frame{_i} {}
			temp(Packet& p): frame{p.data()} {}
		};

		struct trip: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t tripCode; uint32_t module;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			trip(): frame{0} {}
			trip(uint64_t &_i): frame{_i} {}
			trip(Packet& p): frame{p.data()} {}
		};

		struct last_trip: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					int32_t tripCode; uint32_t module;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			last_trip(): frame{0} {}
			last_trip(uint64_t &_i): frame{_i} {}
			last_trip(Packet& p): frame{p.data()} {}
		};

		struct trip_pt_current: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float low, high;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			trip_pt_current(): frame{0} {}
			trip_pt_current(uint64_t &_i): frame{_i} {}
			trip_pt_current(Packet& p): frame{p.data()} {}
		};

		struct trip_pt_voltage: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float low, high;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			trip_pt_voltage(): frame{0} {}
			trip_pt_voltage(uint64_t &_i): frame{_i} {}
			trip_pt_voltage(Packet& p): frame{p.data()} {}
		};

		struct trip_pt_temp: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					float low, high;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			trip_pt_temp(): frame{0} {}
			trip_pt_temp(uint64_t &_i): frame{_i} {}
			trip_pt_temp(Packet& p): frame{p.data()} {}
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
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			mppt1(): frame{0} {}
			mppt1(uint64_t &_i): frame{_i} {}
			mppt1(Packet& p): frame{p.data()} {}
		};

		struct mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			mppt2(): frame{0} {}
			mppt2(uint64_t &_i): frame{_i} {}
			mppt2(Packet& p): frame{p.data()} {}
		};

		struct mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			mppt3(): frame{0} {}
			mppt3(uint64_t &_i): frame{_i} {}
			mppt3(Packet& p): frame{p.data()} {}
		};
}
 namespace tx {

		struct mppt1: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			mppt1(): frame{0} {}
			mppt1(uint64_t &_i): frame{_i} {}
			mppt1(Packet& p): frame{p.data()} {}
		};

		struct mppt2: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			mppt2(): frame{0} {}
			mppt2(uint64_t &_i): frame{_i} {}
			mppt2(Packet& p): frame{p.data()} {}
		};

		struct mppt3: public Packet {
			union frame_t {
				uint64_t data;
				uint8_t bytes[8];
				struct PACKED members_t {
					;
				} s;
			} frame;
			ALWAYSINLINE uint64_t &data() {return frame.data;}
			ALWAYSINLINE uint8_t  *bytes() {return frame.bytes;}
			mppt3(): frame{0} {}
			mppt3(uint64_t &_i): frame{_i} {}
			mppt3(Packet& p): frame{p.data()} {}
		};
}
}
}
 namespace addr {
 namespace sw {
 namespace rx {
 enum addrs {
lights_k = 768};
}
 namespace tx {
 enum addrs {
heartbeat_k = 784,
error_k,
buttons_k,
lights_k};
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
motor_id_k = 1280,
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
}
}
}

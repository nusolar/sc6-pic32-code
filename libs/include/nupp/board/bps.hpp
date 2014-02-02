/*
 * File:   bps.hpp
 * Author: alex
 *
 * Created on August 30, 2013, 5:52 PM
 */

#ifndef NU_BPS_HPP
#define	NU_BPS_HPP

#include "nupp/board/nu32.hpp"
#include "nupp/comm/voltagesensor.hpp"
#include "nupp/comm/currentsensor.hpp"
#include "nupp/comm/tempsensor.hpp"
#include "nupp/component/nokia5110.hpp"
#include "nupp/component/button.hpp"
#include "nupp/peripheral/usbhid.hpp"
#include "nupp/peripheral/serial.hpp"
#include "nupp/peripheral/spi.hpp"
#include "nupp/peripheral/can.hpp"
#include "nupp/peripheral/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/wdt.hpp"
#include "nupp/array.hpp"
#include "nupp/closure.hpp"
#include "nu/compiler.h"

#define NU_BPS_TIMEOUT_INT_MS 100 //ms
#define NU_BPS_PRECHARGE_TIME_MS 2000 //ms
#define NU_BPS_CONVERSION_TIME_MS 2.3 //ms - time for LTC6804s to complete conversion.

namespace nu {
	/*
	struct BpsMode {
		struct Mode {
			int id;
			bool can_discharge, can_charge, drive;
			int charge_conj, discharge_conj, drive_conj;
		};

		enum ModeName {
			Off = 0,
			Discharging = 1,
			Drive = 3,
			EmptyCharging = 4,
			Charging = 5,
			DriveCharging = 7,

			NOTHING = -1
		};

		Mode modes[6];
		int current_mode;

		INLINE BpsMode(): current_mode(Off) {
			modes[Off] = {Off,
				false, false, false,
				EmptyCharging, Discharging, NOTHING};
			modes[Discharging] = {Discharging,
				true, false, false,
				Charging, Off, Drive};
			modes[Drive] = {Drive,
				true, true, false,
				DriveCharging, Off, Discharging};
			modes[EmptyCharging] = {EmptyCharging,
				false, false, true,
				Off, Charging, NOTHING};
			modes[Charging] = {Charging,
				true, false, true,
				Discharging, EmptyCharging, DriveCharging};
			modes[DriveCharging] = {DriveCharging,
				true, true, true,
				Drive, EmptyCharging, Charging};
		}

		INLINE void traverse_mode(bool health, bool can_discharge, bool can_charge, bool drive) {
			if (!health) {
				current_mode = Off;
			} else {
				if (can_discharge != modes[current_mode].can_charge) {
					// FIRST kill discharge if needed.
					current_mode = modes[current_mode].discharge_conj;
				} else if (can_charge != modes[current_mode].can_charge) {
					// THEN toggle charging, if needed.
					current_mode = modes[current_mode].charge_conj;
				} else if (drive != modes[current_mode].drive) {
					// FINALLY after discharge & charge are correct, drive.
					if (modes[current_mode].drive_conj != NOTHING) {
						// only drive when drive_conj != NOTHING
						current_mode = modes[current_mode].drive_conj;
					}
				}
			}
		}
	};*/

 	struct BPS: protected Nu32 {
		static const size_t N_MODULES = 32;
		static const uint16_t MAX_VOLTAGE = 43000; // multiple of 100uV
		static const uint16_t MIN_VOLTAGE = 27500; // 100uV
		static const uint32_t MAX_TEMP = 45;
		static const uint32_t MIN_TEMP = 0;
		static const float MAX_BATT_CURRENT_DISCHARGING = 72; // 72.8
		static const float MAX_BATT_CURRENT_CHARGING = -32; // 36.4

		static const uint16_t MIN_DISCHARGE_VOLTAGE = MIN_VOLTAGE + 100;
		static const uint16_t MAX_CHARGE_VOLTAGE = MAX_VOLTAGE + 10;

		enum TripCode {
			NONE = 0,
			OVER_VOLT,
			UNDER_VOLT,
			OVER_TEMP,
			UNDER_TEMP,
			OVER_CURR_DISCHARGING,
			OVER_CURR_CHARGING
		};

		enum Modes {
			OFF = 0,
			DISCHARGING = 1,
			DRIVE = 2,

			CHARGING = 5,
			CHARGING_DRIVE = 6
		};

		enum Precharge {
			PC_OFF = 0,
			PC_CHARGING = 1,
			PC_CHARGED = 2
		};

		enum Errors {
			NOERR = 0,
			BADMODE,
			BADPC
		};

		DigitalOut main_relay, array_relay, precharge_relay, motor_relay;
		Button bypass_button;
		Serial com;
		can::Module common_can;
		Nokia5110 lcd1, lcd2;

#warning "CONFIG in sensor class definitions"
		VoltageSensor voltage_sensor; //on D9, SPI Chn 1
		CurrentSensor current_sensor; // on F12, SPI Chn 4 
		TempSensor temp_sensor; //on A0

		Timer mode_timeout_clock;
		Timer precharge_timer;
		Timer lcd_timer;

		/**
		 * Aggregated data of the Sensors and the BMS.
		 */
		struct State {

			// scientific data
			double cc_battery, cc_array, wh_battery, wh_array;
			int16_t current0, current1; // 10bit voltage, 0-5V
			Array<uint16_t, 36> voltages; // in 100uv. 4 extra voltages, for empty LTC slots
			Array<float, N_MODULES> temperatures; // in degC

			// Trip Code & data
			TripCode trip_code;
			int8_t trip_data;

			// Protection modes & timers
			Modes mode;
			Precharge precharging;

			// error data
			int8_t disabled_module;
			int8_t last_error;
			int8_t error;
			int8_t error_value;

			// timers & indices used in the Run Loop
			uint32_t uptime; //s
			uint8_t module_i;

			INLINE State(): cc_battery(0), cc_array(0), wh_battery(0), wh_array(0),
				current0(0), current1(0), voltages(0), temperatures(0),
				trip_code(NONE), trip_data(0),
				mode(OFF), precharging(PC_OFF),
				disabled_module(-1), last_error(0), error(NOERR), error_value(0),
				uptime(0), module_i(0) {}
		} state;

		/**
		 * The BPS's HID report.
		 */
		/*struct Report {
			// scientific data
			double cc_battery, cc_array, wh_battery, wh_array;
			int16_t current0, current1; // 10bit voltage, 0-5V
			uint16_t voltages[32]; // in 100uv
			float temperatures[32];
			// protection data
			uint8_t mode, precharge;
			// error data
			int8_t disabled_module;
			int8_t last_error;
			int8_t error;
			int8_t error_value;

			INLINE Report (State s): cc_battery(s.cc_battery), cc_array(s.cc_array),
				wh_battery(s.wh_battery), wh_array(s.wh_array),
				current0(s.current0), current1(s.current1),
				voltages(), temperatures(),
				mode(s.mode), precharge(s.precharging),
				disabled_module(s.disabled_module), last_error(s.last_error),
				error(s.error), error_value(s.error_value)
			{
				memcpy(voltages, s.voltages.data(), sizeof(voltages));
				memcpy(temperatures, s.temperatures.data(), sizeof(temperatures));
			}
		};*/


		INLINE BPS(): Nu32(Nu32::V2011),
			main_relay(PIN(D, 1), false),
			array_relay(PIN(D, 2), false),
			precharge_relay(PIN(D, 3), false),
			motor_relay(PIN(D, 4), false),
			bypass_button(PIN(B, 7)),
			com(UART(1)),
			common_can(CAN1),
			lcd1(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9),  PIN(E, 9)),
			lcd2(PIN(E, 8), SPI_CHANNEL2, PIN(A, 10), PIN(E, 9)),
			voltage_sensor(),
			current_sensor(),
			temp_sensor(),
			mode_timeout_clock(NU_BPS_TIMEOUT_INT_MS, Timer::ms, true),
			precharge_timer(NU_BPS_PRECHARGE_TIME_MS, Timer::ms, false),
			lcd_timer(1, Timer::s, true),
			state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);

			// Initialize sensors,
			this->read_ins();
		}

		INLINE void read_ins() {
			// debounce bypass button
			for (unsigned i=0; i<10; i++) {
				bypass_button.update();
			}
			// increment disabled module if it's pressed
			if (bypass_button.toggled()) {
				++state.disabled_module;
				if (state.disabled_module == (int8_t)N_MODULES) {
					state.disabled_module = -1;
				}
			}

			voltage_sensor.read();
			current_sensor.read();
			temp_sensor.read();
		}

		INLINE void recv_can() {
			can::frame::Packet pkt(0);
			uint32_t id;
			common_can.in().rx(pkt,id);

			switch (id) {
				case can::frame::os::tx::driver_input_k: {
					can::frame::os::tx::driver_input data(pkt);
					state.mode = (Modes)data.frame().power;
					this->mode_timeout_clock.reset();
				}
				default: {
				}
			}

			// If state.mode_timeout_clock isn't updated fast enough, turn OFF.
			if (this->mode_timeout_clock.has_expired()) {
				this->mode_timeout_clock.kill();
				state.mode = OFF;
			}
		}

		/*INLINE void hid_tx_callback(unsigned char *data, size_t len) {
			Report r = Report(state);
			memcpy(data, &r, len);
		}*/

		INLINE void check_trip_conditions() {
			TripCode trip_code = NONE;
			int8_t trip_data = -1;

			for (uint8_t i=0; i<N_MODULES; ++i) {
				if (i == state.disabled_module) {
					continue;
				}
				if (state.voltages[i] > MAX_VOLTAGE) {
					trip_code = OVER_VOLT;
				} else if (state.voltages[i] < MIN_VOLTAGE) {
					trip_code = UNDER_VOLT;
				} else if (state.temperatures[i] > MAX_TEMP) {
					trip_code = OVER_TEMP;
				} else if (state.temperatures[i] < MIN_TEMP) {
					trip_code = UNDER_TEMP;
				} else {
					continue;
				}

				trip_data = i;
				break;
			}

			if (state.current0 > MAX_BATT_CURRENT_DISCHARGING) {
				trip_code = OVER_CURR_DISCHARGING;
				trip_data = 0;
			} else if (state.current0 < MAX_BATT_CURRENT_CHARGING) {
				trip_code = OVER_CURR_CHARGING;
				trip_data = 0;
			}

			state.trip_code = trip_code;
			state.trip_data = trip_data;
		}

		INLINE void check_mode_safety() {
			if (state.trip_code != NONE) {
				state.mode = OFF;
			}
		}

		/**
		 * Sets the Battery Protection Mode, based on state.mode.
		 * Either OFF, a DISCHARGE mode, or CHARGING.
		 *
		 * Be careful, CHARGING mode is permitted in two circumstances:
		 * (1) when batteries are normal, and could potentially go to DRIVE or CHARGING_DRIVE
		 * (2) when batteries are critically low, and could not sustain DISCHARGING/DRIVE.
		 *
		 * If state.mode is not valid, sets state.error.
		 */
		INLINE void set_relays() {
			bool has_error = false;

			switch (state.mode) {
				default:
					has_error = true;
				case OFF: {
					// FIRST: halt major drains
					motor_relay.set(false);
					precharge_relay.set(false);
					// THEN: deactivate batteries
					main_relay.set(false);
					// FINALLY: disconnect array
					array_relay.set(false);
					break;
				}
				case DISCHARGING: {
					// FIRST: ensure NOT charging
					array_relay.set(false);
					// THEN: not draining
					motor_relay.set(false);
					precharge_relay.set(false);
					// FINALLY: discharge
					main_relay.set(true);
					break;
				}
				case DRIVE: {
					// FIRST: ensure NOT charging
					array_relay.set(false);
					// THEN: discharging
					main_relay.set(true);
					// THEN; precharge, & close motor relay when charged.
					// SEE BELOW
				}
				/* CHARGING means batteries could be critically low, or just charging
				 * during normal operation & discharge. */
				case CHARGING: {
					// FIRST: ensure NOT draining
					motor_relay.set(false);
					precharge_relay.set(false);
					// THEN: allow charging
					array_relay.set(true);
					// FINALLY: charge
					main_relay.set(true);
					break;
				}
				case CHARGING_DRIVE: {
					// FIRST: allow charging
					array_relay.set(true);
					// THEN: charge
					main_relay.set(true);
					// FINALLY: precharge & close motor relays
					// SEE BELOW
					break;
				}
			}
			// Set or remove Mode error, if needed.
			if (has_error) {
				if (state.error == NOERR)
					state.error = BADMODE;
			} else if (state.error == BADMODE)
					state.error = NOERR;

			// Now, reset or continue with the precharge sequence:
			if (state.mode != DRIVE && state.mode != CHARGING_DRIVE) {
				state.precharging = PC_OFF;
			} else {
				switch (state.precharging) {
					default:
						has_error = BADPC;
					case PC_OFF: {
						motor_relay.set(false);
						precharge_relay.set(true);
						this->precharge_timer.reset();
						state.precharging = PC_CHARGING;
						break;
					}
					case PC_CHARGING: {
						motor_relay.set(false);
						precharge_relay.set(true);
						if (this->precharge_timer.has_expired()) {
							state.precharging = PC_CHARGED;
						}
						break;
					}
					case PC_CHARGED: {
						motor_relay.set(true);
						precharge_relay.set(false);
						break;
					}
				}
				// Set or remove Precharge error, if needed.
				if (has_error) {
					if (state.error==NOERR)
						state.error = BADPC;
				} else if (state.error==BADPC)
					state.error = NOERR;

			}
		}

		INLINE void send_can() {
			// state.time = timer::ms();
		}

		INLINE void draw_lcd() {
			if (this->lcd_timer.has_expired()) {
				lcd1.lcd_clear();
				lcd1 << "ZELDA " << state.module_i << end;
				lcd1.goto_xy(0, 1);
				lcd1 << "V: " << state.voltages[state.module_i] << end;
				lcd1.goto_xy(0, 2);
				lcd1 << "T: " << state.temperatures[state.module_i] << end;
				lcd1.goto_xy(0, 3);
				lcd1 << "I0: " <<  state.current0 << end;
				lcd1.goto_xy(0, 4);
				lcd1 << "Off: " << state.disabled_module << end;
				lcd1.goto_xy(0, 5);
				lcd1 << "R: " << main_relay.status() << "-" << array_relay.status() <<
						"-" << precharge_relay.status() << "-" << motor_relay.status() << end;
				led1.toggle();

				++state.module_i %= N_MODULES; // next module index
				++state.uptime; // increase uptime by 1 second
				this->lcd_timer.reset(); // reset LCD update clock
			}
		}

		INLINE void emergency_shutoff() {
			main_relay.low();
			array_relay.low();
			motor_relay.low();
			precharge_relay.low();
		}

		/**
		 * The main run loop.
		 */
		INLINE NORETURN void run_loop() {
			while (true) {
				WDT::clear();
				this->recv_can();
				this->read_ins();
				this->check_trip_conditions();
				this->check_mode_safety();
				this->set_relays();
				this->draw_lcd();
			}
		}

		static NORETURN void main(BPS *bps);
	};
}

#endif	/* NU_BPS_HPP */


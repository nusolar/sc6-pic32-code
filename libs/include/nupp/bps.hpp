/* 
 * File:   bps.hpp
 * Author: alex
 *
 * Created on August 30, 2013, 5:52 PM
 */

#ifndef NU_BPS_HPP
#define	NU_BPS_HPP

#include "nupp/nu32.hpp"
#include "nupp/ds18x20.hpp"
#include "nupp/nokia5110.hpp"
#include "nupp/serial.hpp"
#include "nupp/spi.hpp"
#include "nupp/can.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/array.hpp"
#include "nu/compiler.h"
#include "nupp/json.hpp"
#include <string>

#define NU_TIMEOUT_INT 100 //ms
#define NU_PRECHARGE_TIME 2500 //ms

namespace nu {
 	struct BPS: protected Nu32 {
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

		DigitalOut main_relay, array_relay, precharge_relay, motor_relay;
		DigitalIn bits[6];
		Serial usb;
		can::Module common_can;
		Nokia5110 lcd1, lcd2;

		AnalogIn current_sensor0, current_sensor1; // Current ADCs
		// LTC6804
		DS18X20<32> temp_sensor; //on A0

		/**
		 * The state of the Batteries and the BMS.
		 */
		struct state {
			static const size_t num_modules = 32;
			reg_t last_error;
			uint8_t disabled_module;
			const char *msg;
			
			uint64_t time;
			uint64_t lcd_clock; //ms
			uint8_t module_i;
			
			double cc_battery, cc_array, wh_battery, wh_array;
			reg_t current0, current1;

			Modes mode, target_mode;
			uint64_t timeout_clock; //ms
			Precharge precharging;
			uint64_t precharge_clock; //ms
			

			state(): last_error(0), disabled_module(63), msg(""),
				time(0), lcd_clock(0), module_i(0),
				cc_battery(0), cc_array(0), wh_battery(0), wh_array(0),
				current0(0), current1(0),
				mode(OFF), target_mode(OFF), timeout_clock(0), precharging(PC_OFF), precharge_clock(0) {}
		} state;

		ALWAYSINLINE BPS(): Nu32(Nu32::V2011),
			main_relay(PIN(D, 2), false),
			array_relay(PIN(D, 3), false),
			precharge_relay(PIN(D, 4), false),
			motor_relay(PIN(D, 5), false),
			bits(),
			usb(UART(1)),
			common_can(CAN1),
			lcd1(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9),  PIN(E, 9)),
			lcd2(PIN(E, 8), SPI_CHANNEL2, PIN(A, 10), PIN(E, 9)),
			current_sensor0(PIN(B, 0)),
			current_sensor1(PIN(B, 1)),
			// voltage_sensor(PIN(D, 9), SPI_CHANNEL1),
			temp_sensor(PIN(A, 0)),
			state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);

			// populate data
			// voltage_sensor.write_configs(cfg);
			temp_sensor.perform_temperature_conversion();
		}

		ALWAYSINLINE void read_ins() {
			state.disabled_module = 0;
			for (unsigned i=0; i<6; i++) {
				state.disabled_module |= (uint8_t)(((bool)bits[i].read()) << i);
			}

			// voltage_sensor.update_volts();
			state.current0 = current_sensor0.read();
			state.current1 = current_sensor1.read();
			temp_sensor.update_temperatures();

			// repopulate data
			// voltage_sensor.write_configs(cfg);
			temp_sensor.perform_temperature_conversion();
		}

		/** USES HEAP */
		ALWAYSINLINE void serial_io() {
			static Json::FastWriter fw = Json::FastWriter();
			static Json::Reader reader = Json::Reader();

			// assemble data
			Json::Value report;
			report["battery_current"] = state.current0;
			report["array_current"] = state.current1;
			report["disabled_module"] = state.disabled_module;
			report["uptime"] = state.time;
			report["mode"] = state.mode;

			// transmit as Single line of JSON
			std::string report_str = fw.write(report);
			usb.tx(report_str.data(), report_str.length());

			Array<char, 30> input;
			int length = usb.read_line(input, input.size());
			if (length > 0) {
				state.timeout_clock = timer::ms();
				try {
					reader.parse(input.data(), report);
					state.target_mode = (Modes)report["mode"].asInt();
				} catch (...) {
					length = 0;
				}
			}
			
			if (length == 0) {
				uint64_t time = timer::ms();
				if (time > NU_TIMEOUT_INT + (time>state.timeout_clock? state.timeout_clock: 0)) {
					state.target_mode = OFF;
				}
			}
		}

		/**
		 * Set the Battery Protection Mode, based on state.target_mode.
		 * Either OFF, a DISCHARGE mode, or CHARGING.
		 *
		 * Be careful, CHARGING mode is permitted in two circumstances:
		 * (1) when batteries are normal, and could potentially go to DRIVE or CHARGING_DRIVE
		 * (2) when batteries are critically low, and could not sustain DISCHARGING/DRIVE.
         */
		ALWAYSINLINE void set_mode() {
			switch (state.target_mode) {
				case OFF: {
					// FIRST: halt major drains
					motor_relay = false;
					precharge_relay = false;
					// THEN: deactivate batteries
					main_relay = false;
					// FINALLY: disconnect array
					array_relay = false;
					break;
				}
				case DISCHARGING: {
					// FIRST: ensure NOT charging
					array_relay = false;
					// THEN: not draining
					motor_relay = false;
					precharge_relay = false;
					// FINALLY: discharge
					main_relay = true;
					break;
				}
				case DRIVE: {
					// FIRST: ensure NOT charging
					array_relay = false;
					// THEN: discharging
					main_relay = true;
					// THEN; precharge, & close motor relay when charged.
					// SEE BELOW
				}
				/* CHARGING means batteries could be critically low, or just charging
				 * during normal operation & discharge. */
				case CHARGING: {
					// FIRST: ensure NOT draining
					motor_relay = false;
					precharge_relay = false;
					// THEN: allow charging
					array_relay = true;
					// FINALLY: charge
					main_relay = true;
					break;
				}
				case CHARGING_DRIVE: {
					// FIRST: allow charging
					array_relay = true;
					// THEN: charge
					main_relay = true;
					// FINALLY: precharge & close motor relays
					// SEE BELOW
					break;
				}
				default:
					state.msg = "BADMODE";
					break;
			}

			// Now, reset or continue with the precharge sequence:
			if (state.target_mode != DRIVE && state.target_mode != CHARGING_DRIVE) {
				state.precharging = PC_OFF;
			} else {
				switch (state.precharging) {
					case PC_OFF: {
						motor_relay = false;
						precharge_relay = true;
						state.precharge_clock = timer::ms();
						state.precharging = PC_CHARGING;
						break;
					}
					case PC_CHARGING: {
						motor_relay = false;
						precharge_relay = true;
						uint64_t time = timer::ms();
						if (time > NU_PRECHARGE_TIME + (time>state.precharge_clock? state.precharge_clock: 0)) {
							state.precharging = PC_CHARGED;
						}
						break;
					}
					case PC_CHARGED: {
						motor_relay = true;
						precharge_relay = false;
						break;
					}
					default:
						state.msg = "BADPC";
						break;
				}
			}

			state.mode = state.target_mode;
		}

		ALWAYSINLINE void emergency_shutoff() {
			main_relay.low();
			array_relay.low();
			motor_relay.low();
			precharge_relay.low();
		}
		
		/**
		 * The main run loop.
		 */
		ALWAYSINLINE NORETURN void run_loop() {
			while (true) {
				WDT::clear();
				
				this->read_ins();
				this->serial_io();
				this->set_mode();

				state.time = timer::ms();
				if (state.time<state.lcd_clock || state.time - state.lcd_clock > 1000) {
					lcd1.lcd_clear();
					lcd1 << "ZELDA " << state.module_i << end;
					lcd1.goto_xy(0, 1);
					lcd1 << "V: " /*<< voltage_sensor[state.module_i]*/ << end;
					lcd1.goto_xy(0, 2);
					lcd1 << "T: " << temp_sensor[state.module_i] << end;
					lcd1.goto_xy(0, 3);
					lcd1 << "I0: " <<  state.current0 << end;
					lcd1.goto_xy(0, 4);
					lcd1 << "Off: " << state.disabled_module << end;
					lcd1.goto_xy(0, 5);
					lcd1 << "R: " << main_relay.status() << "-" << array_relay.status() <<
							"-" << precharge_relay.status() << "-" << motor_relay.status() << end;
					led1.toggle();

					++state.module_i %= 32;
					state.lcd_clock = state.time;
				}
			}
		}

		static NORETURN void main();
	};

	extern BPS bps;
}

#endif	/* NU_BPS_HPP */


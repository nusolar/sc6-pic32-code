/* 
 * File:   bps.hpp
 * Author: alex
 *
 * Created on August 30, 2013, 5:52 PM
 */

#ifndef NU_BPS_HPP
#define	NU_BPS_HPP

#include "nupp/board/nu32.hpp"
#include "nupp/component/ltc6804.hpp"
#include "nupp/component/ds18x20.hpp"
#include "nupp/component/nokia5110.hpp"
#include "nupp/usbhid.hpp"
#include "nupp/serial.hpp"
#include "nupp/spi.hpp"
#include "nupp/can.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/wdt.hpp"
#include "nupp/array.hpp"
#include "nupp/closure.hpp"
#include "nu/compiler.h"

#define NU_BPS_MAX_VOLTAGE 4300 // mV
#define NU_BPS_MIN_VOLTAGE 2750 // mV
#define NU_BPS_TIMEOUT_INT 100 //ms
#define NU_BPS_PRECHARGE_TIME 2000 //ms
#define NU_BPS_CONVERSION_TIME 2.3 //ms - time for LTC6804s to complete conversion.

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

		enum Errors {
			NOERR = 0,
			BADMODE,
			BADPC
		};

		DigitalOut main_relay, array_relay, precharge_relay, motor_relay;
		DigitalIn bits[6];
		UsbHid hid;
		Serial com;
		can::Module common_can;
		Nokia5110 lcd1, lcd2;

		AnalogIn current_sensor0, current_sensor1; // Current ADCs
		LTC6804<3> voltage_sensor; //on D9, SPI Chn 1
		DS18X20<32> temp_sensor; //on A0

		/**
		 * Aggregated data of the Sensors and the BMS.
		 */
		struct State {
			static const size_t num_modules = 32;
			
			// scientific data
			double cc_battery, cc_array, wh_battery, wh_array;
			int16_t current0, current1; // 10bit voltage, 0-5V
			Array<uint16_t, 36> voltages; // in 100uv. 4 extra voltages, for empty LTC slots
			Array<float, num_modules> temperatures; // in degC

			// Protection modes & timers
			Modes mode;
			Precharge precharging;
			uint64_t mode_timeout_clock; //ms
			uint64_t precharge_clock; //ms

			// error data
			uint8_t disabled_module;
			int8_t last_error;
			int8_t error;
			int8_t error_value;
			
			// timers & indices used in the Run Loop
			uint64_t time;
			uint64_t lcd_clock; //ms
			uint8_t module_i;
			
			State(): cc_battery(0), cc_array(0), wh_battery(0), wh_array(0),
				current0(0), current1(0), voltages(0), temperatures(0),
				mode(OFF), precharging(PC_OFF), mode_timeout_clock(0), precharge_clock(0),
				disabled_module(63), last_error(0), error(NOERR), error_value(0),
				time(0), lcd_clock(0), module_i(0) {}
		} state;

		/**
		 * The BPS's HID report.
		 */
		struct Report {
			// scientific data
			double cc_battery, cc_array, wh_battery, wh_array;
			int16_t current0, current1; // 10bit voltage, 0-5V
			uint16_t voltages[32]; // in 100uv
			float temperatures[32];
			// protection data
			uint8_t mode, precharge;
			// error data
			uint8_t disabled_module;
			int8_t last_error;
			int8_t error;
			int8_t error_value;

			Report (State s): cc_battery(s.cc_battery), cc_array(s.cc_array),
				wh_battery(s.wh_battery), wh_array(s.wh_array),
				current0 (s.current0), current1(s.current1),
				voltages(), temperatures(),
				mode(s.mode), precharge(s.precharging),
				disabled_module(s.disabled_module), last_error(s.last_error),
				error(s.error), error_value(s.error_value)
			{
				memcpy(voltages, s.voltages.data(), sizeof(voltages));
				memcpy(temperatures, s.temperatures.data(), sizeof(temperatures));
			}
		};


		ALWAYSINLINE BPS(): Nu32(Nu32::V2011),
			main_relay(PIN(D, 2), false),
			array_relay(PIN(D, 3), false),
			precharge_relay(PIN(D, 4), false),
			motor_relay(PIN(D, 5), false),
			bits(),
			hid(),
			com(UART(1)),
			common_can(CAN1),
			lcd1(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9),  PIN(E, 9)),
			lcd2(PIN(E, 8), SPI_CHANNEL2, PIN(A, 10), PIN(E, 9)),
			current_sensor0(PIN(B, 0)),
			current_sensor1(PIN(B, 1)),
			voltage_sensor(PIN(D, 9), SPI_CHANNEL1),
			temp_sensor(PIN(A, 0)),
			state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);

			// configure sensors
			this->configure_sensors();

			// begin conversion
			voltage_sensor.start_voltage_conversion();
			temp_sensor.perform_temperature_conversion();
		}

		ALWAYSINLINE void configure_sensors() {
			// send random data, to wake LTCs
			voltage_sensor.cs.low();
			voltage_sensor.tx(&state.mode, 1);
			voltage_sensor.cs.high();
			timer::delay_ms(3);

			// prepare LTC configuration
			LTC6804<3>::Configuration cfg0 = {0};
			cfg0.bits.adcopt = 0; // normal ADC mode
			cfg0.bits.refon = 1; // stay on
			cfg0.bits.vuv = (voltage_sensor.convert_uv_limit(NU_BPS_MIN_VOLTAGE)+1) & 0xfff; // mask 12 bits
			cfg0.bits.vov = voltage_sensor.convert_ov_limit(NU_BPS_MAX_VOLTAGE) & 0xfff;

			// write LTC6804 config register, also resetting LTC's WatchDogTimer
			Array<LTC6804<3>::Configuration, 3> cfgs;
			cfgs = cfg0;
			voltage_sensor.write_configs(cfgs);
		}

		ALWAYSINLINE void read_ins() {
			// read on-board switches.
			state.disabled_module = 0;
			for (unsigned i=0; i<6; i++) {
				state.disabled_module |= (uint8_t)(((bool)bits[i].read()) << i);
			}

			state.current0 = current_sensor0.read() & 0x3ff; //mask 10 bits
			state.current1 = current_sensor1.read() & 0x3ff;
			temp_sensor.update_temperatures(state.temperatures);
			voltage_sensor.read_volts(state.voltages);
			

			// repopulate data
			temp_sensor.perform_temperature_conversion();
			voltage_sensor.start_voltage_conversion();
		}

		ALWAYSINLINE void serial_io() {
			com << TTY_UNIT << "battery_current" << TTY_RECORD << state.current0 << nu::end;
			com << TTY_UNIT << "array_current" << TTY_RECORD << state.current1 << nu::end;
			com << TTY_UNIT << "disabled_module" << TTY_RECORD << state.disabled_module << nu::end;
			com << TTY_UNIT << "uptime" << TTY_RECORD << state.time << nu::end;
			com << TTY_UNIT << "mode" << TTY_RECORD << state.mode << TTY_UNIT << nu::end;

			// get line
			Array<char, 30> input;
			int length = com.read_line(input, input.size());

			if (length > 0 &&
				input[0] == TTY_UNIT &&
				strncmp(input.data()+1, "mode", 4) == 0 &&
				input[5] == TTY_RECORD)
			{
				char mode[2] = {input[6], '\0'};
				state.mode = (Modes)atoi(mode);
			} else {
				length = 0;
			}

			// If handling failed, and if timeout overflowed, turn OFF.
			// Else if handling succeeded, reset timeout_clock
			if (length == 0) {
				uint64_t time = timer::ms();
				if (time > NU_BPS_TIMEOUT_INT + (time>state.mode_timeout_clock? state.mode_timeout_clock: 0)) {
					state.mode = OFF;
				}
			} else {
				state.mode_timeout_clock = timer::ms();
			}
		}

		void hid_tx_callback(unsigned char *data, size_t len) {
			Report r = Report(state);
			memcpy(data, &r, len);
		}

		void hid_rx_callback(unsigned char *data, size_t len) {
			if (len > 0) {
				state.mode = (Modes)data[0];
				state.mode_timeout_clock = timer::ms();
			}
		}

		ALWAYSINLINE void do_hid() {
			typedef Closure<typeof(*this), typeof(&nu::BPS::hid_tx_callback), void, unsigned char *, size_t> delegate;
			hid.try_tx(delegate::use(*this, &nu::BPS::hid_tx_callback));
			hid.try_rx(delegate::use(*this, &nu::BPS::hid_rx_callback));

			// If state.mode_timeout_clock isn't updated fast enough, turn OFF.
			uint64_t time = timer::ms();
			if (time > NU_BPS_TIMEOUT_INT + (time>state.mode_timeout_clock? state.mode_timeout_clock: 0)) {
				state.mode = OFF;
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
			bool has_error = false;
			
			switch (state.mode) {
				default:
					has_error = true;
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
						if (time > NU_BPS_PRECHARGE_TIME + (time>state.precharge_clock? state.precharge_clock: 0)) {
							state.precharging = PC_CHARGED;
						}
						break;
					}
					case PC_CHARGED: {
						motor_relay = true;
						precharge_relay = false;
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

				this->configure_sensors();
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
					lcd1 << "T: " << state.temperatures[state.module_i] << end;
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

		static NORETURN void main(BPS *bps);
	};
}

#endif	/* NU_BPS_HPP */


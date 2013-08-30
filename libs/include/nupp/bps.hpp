/* 
 * File:   bps.hpp
 * Author: alex
 *
 * Created on August 30, 2013, 5:52 PM
 */

#ifndef NU_BPS_HPP
#define	NU_BPS_HPP

#include "nupp/spi.hpp"
#include "nupp/can.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nu/compiler.h"
#include "serial.hpp"

namespace nu {
	struct BPS: protected Nu32 {
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
			
			uint64_t time;
			uint64_t lcd_clock;
			uint8_t module_i;
			
			double cc_battery, cc_array, wh_battery, wh_array;
			reg_t current0, current1;
			
			bool main_relay, array_relay, precharge_relay, motor_relay;

			state(): last_error(0), disabled_module(63),
				time(0), lcd_clock(0), module_i(0),
				cc_battery(0), cc_array(0), wh_battery(0), wh_array(0),
				current0(0), current1(0),
				main_relay(false), array_relay(false),
				precharge_relay(false), motor_relay(false) {}
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
			temp_sensor.perform_temperature_conversion();
		}

		ALWAYSINLINE void read_ins() {
			// voltage_sensor.write_configs(cfg);

			state.disabled_module = 0;
			for (unsigned i=0; i<6; i++) {
				state.disabled_module |= (uint8_t)(((bool)bits[i].read()) << i);
			}

			// voltage_sensor.update_volts();
			state.current0 = current_sensor0.read();
			state.current1 = current_sensor1.read();
			temp_sensor.update_temperatures();

			// repopulate data
			temp_sensor.perform_temperature_conversion();
		}

		ALWAYSINLINE void serial_io() {
			// TODO Pack battery data, transmit
			usb.tx("{}", 3);
			// TODO receive relay commands
		}

		ALWAYSINLINE void set_relays() {
			// TODO validate relay commands
			main_relay = state.main_relay;
			array_relay = state.array_relay;
			precharge_relay = state.precharge_relay;
			motor_relay = state.motor_relay;
		}
		
		/**
		 * The main run loop.
		 */
		ALWAYSINLINE NORETURN void run_loop() {
			while (true) {
				WDT::clear();
				
				this->read_ins();
				this->serial_io();
				this->set_relays();

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
	};
}

#endif	/* NU_BPS_HPP */


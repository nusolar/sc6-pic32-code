//
//  bms.h
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__bms__
#define __nusolar_lib__bms__

#include "nu/compiler.h"
#include "nupp/nokia5110.hpp"
#include "nupp/hais.hpp"
#include "nupp/ad7685.hpp"
#include "nupp/ltc6803.hpp"
#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"
#include <cstdint>

namespace nu {
	/**
	 * Battery Management System. Controls voltage, temperature, and current
	 * monitoring sensors; informs telemetry; emergency-stops the car if needed.
	 * TODO: Implement.
	 */
	struct BatteryMs: protected Nu32 {
		enum tripcodes {
			NONE = 0,
			OTHER,
			OW_BUS_FAILURE,
			DS18X20_MISSING,
			LTC_POST_FAILED,
			OVER_VOLTAGE,
			UNDER_VOLTAGE,
			OVER_CURRENT_DISCHARGE,
			OVER_CURRENT_CHARGE,
			OVER_TEMP,
			UNDER_TEMP // sanity check
		};

		DigitalOut main_relay, array_relay;
		DigitalIn bits[6];
		can::Module common_can, mppt_can;
		Nokia5110 lcd1, lcd2;
		HAIS<2> current_sensor; // 2 ADCs
		LTC6803<3> voltage_sensor; // 3 LTCs
		// DS18B20 on A0


		/**
		 * The state of the Batteries and the BMS.
		 */
		struct state {
			static const size_t num_modules = 32;
			double uptime;
			uint32_t last_trip_module;
			uint8_t disabled_module;
			float voltages[num_modules], temperatures[num_modules], openwire_voltages[num_modules];
			float current_battery, current_array;
			double cc_battery, cc_array, wh_battery, wh_array;
			double cc_mppt[3], wh_mppt_in[3], wh_mppt_out[3];
		} state;


		/**
		 * Setup 2 Relays, 2 CAN modules, 2 Nokia LCDs, 2 Current sensors,
		 * 3 Voltage sensor modules, 32 Temperature sensors, and more.
		 * TODO: Lots
		 */
		ALWAYSINLINE BatteryMs(): Nu32(Nu32::V1), 
			main_relay(Pin(Pin::D, 2), false),
			array_relay(Pin(Pin::D, 3), false),
			bits(),
			common_can(CAN1), mppt_can(CAN2),
			lcd1(Pin(Pin::G, 9), SPI_CHANNEL2, Pin(Pin::A, 9), Pin(Pin::E, 9)),
			lcd2(Pin(Pin::E, 8), SPI_CHANNEL2, Pin(Pin::A, 10), Pin(Pin::E, 9)),
			current_sensor(
				AD7685<2>(Pin(Pin::F, 12), SPI_CHANNEL4, Pin(Pin::F, 12), // Convert & CS are same pin
				AD7685<2>::CHAIN_MODE_NO_BUSY), HAIS<2>::P50),
			voltage_sensor(Pin(Pin::D, 9), SPI_CHANNEL1), state()
		{
			WDT::clear();
			state.last_trip_module = 12345;
			state.disabled_module = 63;

			for (uint8_t i=0; i<6; i++) {
				bits[i] = DigitalIn(Pin(Pin::B, i));
			}

			common_can.in().setup_rx();
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);
			mppt_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}


		ALWAYSINLINE void read_ins() {
			state.disabled_module = 0;
			for (unsigned i=0; i<6; i++) {
				state.disabled_module |= (uint8_t)((bool)bits[i].read() >> i);
			}
			voltage_sensor.read_volts();
			current_sensor.read_current();
		}

		ALWAYSINLINE void check_batteries() {
			for (uint8_t i=0; i<state.num_modules; i++) {
				voltage_sensor[i] < 2.75; // TRIP
				voltage_sensor[i] > 4.3; // TRIP
				// temp_sensor[i] > 35;
				// temp_sensor[i] < 0;
			}
			current_sensor[0] > 72.8; // BattADC
			current_sensor[0] < -36.4;
			current_sensor[1] > 10; // ArrayADC
			current_sensor[1] < -1;
		}


		ALWAYSINLINE void send_can() {
			{
				can::frame::bms::tx::current pkt(0);
				pkt.frame.s.array = current_sensor[0]; // Marked "BattADC"
				pkt.frame.s.battery = current_sensor[1]; // Marked "ArrayADC"
				common_can.out().tx(pkt.bytes(), 8, can::addr::bms::tx::current_k);
			}
			{
				can::frame::bms::tx::voltage pkt(0);
				pkt.frame.s.module = 0;
				pkt.frame.s.voltage = voltage_sensor[0];
				common_can.out().tx(pkt.bytes(), 8, can::addr::bms::tx::voltage_k);
			}
			{
				can::frame::bms::tx::temp pkt(0);
				pkt.frame.s.sensor = 0;
				pkt.frame.s.temp = 25; // ERROR TODO
				common_can.out().tx(pkt.bytes(), 8, can::addr::bms::tx::temp_k);
			}
		}


		/**
		 * A function to be called repeatedly
		 */
		ALWAYSINLINE void run() {
			WDT::clear();
//			read_ins();
			send_can();
			check_batteries();
			
			lcd1.lcd_clear();
			lcd1 << "C++WINS" << end;
			lcd1.goto_xy(0, 1);
			lcd1 << "V: " << state.voltages[31] << end;
			lcd1.goto_xy(0, 2);
			lcd1 << "T: " << state.temperatures[31] << end;
			lcd1.goto_xy(0, 3);
			lcd1 << "I: " <<  state.current_battery << end;
			lcd1.goto_xy(0, 4);
			lcd1 << "Off: " << 63 << end;
			lcd1.goto_xy(0, 5);
			lcd1 << "MR: " << main_relay.status() << end;
			
			led1.toggle();
			timer::delay_s<1>();
		}

		ALWAYSINLINE void boot() {
//			read_ins();
			check_batteries();
			main_relay.high();
		}

		ALWAYSINLINE void test() {
			WDT::clear();
			lcd1.lcd_clear();
			lcd1 << "C++WINS" << end;
			led1.toggle();
			timer::delay_s<1>();
		}
		
		static NORETURN void main();
	};
}

#endif /* defined(__nusolar_lib__bms__) */

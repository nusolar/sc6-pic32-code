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
#include "nupp/ad7685.hpp" // ERROR breaks Symbol loading
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
	 * TODO: Implement. (Will use C implementation for now)
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
		can::Module common_can, mppt_can;
		Nokia5110 lcd1, lcd2;
		AD7685<2> current_sensor; // 2 ADCs
		LTC6803<3> voltage_sensor; // 3 LTCs


		/**
		 * The state of the Batteries and the BMS.
		 */
		struct state {
			static const size_t num_modules = 32;
			double uptime;
			uint32_t last_trip_module;
			float voltages[num_modules], temperatures[num_modules], openwire_voltages[num_modules];
			float current_battery, current_array;
			double cc_battery, cc_array, wh_battery, wh_array;
			double cc_mppt[3], wh_mppt_in[3], wh_mppt_out[3];
		} state;


		/**
		 * Setup Relays, CAN modules, Nokia LCDs, Current sensors, Voltage sensors,
		 * Temperature sensors, and more.
		 * TODO: Lots
		 */
		ALWAYSINLINE BatteryMs(): Nu32(Nu32::V1), main_relay(Pin(Pin::D, 2)),
			array_relay(Pin(Pin::D, 3)), common_can(CAN1), mppt_can(CAN2),
			lcd1(Pin(Pin::G, 9), SPI_CHANNEL2, Pin(Pin::A, 9), Pin(Pin::E, 9)),
			lcd2(Pin(Pin::E, 8), SPI_CHANNEL2, Pin(Pin::A, 10), Pin(Pin::E, 9)),
			current_sensor (Pin(Pin::A, 0), SPI_CHANNEL4, Pin(Pin::F, 12),
				 (AD7685<2>::options) (AD7685<2>::CHAIN_MODE|AD7685<2>::NO_BUSY_INDICATOR)), // ERROR: SPI pin?
			voltage_sensor(Pin(Pin::D, 9), SPI_CHANNEL1), state()
		{
			WDT::clear();
			state.last_trip_module = 12345;

			main_relay = true;
			array_relay = true;

			common_can.in().setup_rx();
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);

			mppt_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}


		/**
		 * A function to be called repeatedly
		 */
		ALWAYSINLINE void run() {
			lcd1.goto_xy(0, 1);
			lcd1 << "V: %0.9f" << state.voltages[31] << end;
			lcd1.goto_xy(0, 2);
			lcd1 << "T: %0.9f" << state.temperatures[31] << end;
			lcd1.goto_xy(0, 3);
			lcd1 << "I: %0.9f" <<  state.current_battery << end;
			led1.toggle();
			timer::delay_s<1>();
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

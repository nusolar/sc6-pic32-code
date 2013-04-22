//
//  main.cpp
//  car_bms
//
//  Created by Al Chandel on 4/19/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "common_pragmas.h"
#include <cstdint>

#include "nu32.h"
#include "pinctl.h"
#include "nokia5110.h"
#include "can.h"
#include "wdt.h"
#include "ad7685.h"


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
		
		Pin main_relay, array_relay;
		can::Module common_can, mppt_can;
		Nokia5110 lcd1, lcd2;
		AD7685 adc;
		
		
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
		ALWAYSINLINE BatteryMs(): Nu32(Nu32::V2),
			main_relay(IOPORT_D, BIT_2), array_relay(IOPORT_D, BIT_3), common_can(CAN1), mppt_can(CAN2),
			lcd1(SPI(Pin(IOPORT_G, BIT_9), SPI_CHANNEL2), Pin(IOPORT_A, BIT_9), Pin(IOPORT_E, BIT_9)),
			lcd2(SPI(Pin(IOPORT_E, BIT_8), SPI_CHANNEL2), Pin(IOPORT_A, BIT_10), Pin(IOPORT_E, BIT_9)),
			adc (SPI(Pin(IOPORT_A, BIT_0), SPI_CHANNEL4), Pin(IOPORT_F, BIT_12),
			(AD7685::options) (2|AD7685::CHAIN_MODE|AD7685::NO_BUSY_INDICATOR)) // ERROR: SPI pin?
		{
			WDT::clear();
			state.last_trip_module = 12345;
			
			main_relay.set_digital_out();
			array_relay.set_digital_out();
			main_relay.set();
			array_relay.set();
		}
		
		
		/**
		 * A function to be called repeatedly
		 */
		void ALWAYSINLINE run() {
			lcd2.goto_xy(0, 1);
			lcd2.printf("V:%0.9f", state.voltages[31]);
			lcd2.goto_xy(0, 2);
			lcd2.printf("T:%0.9f", state.temperatures[31]);
			lcd2.goto_xy(0, 3);
			lcd2.printf("I:%0.9f", state.current_battery);
		}
	};
}


using namespace std;
using namespace nu;

int main(int argc, const char * argv[]){
	BatteryMs bms{};
    return 0;
}


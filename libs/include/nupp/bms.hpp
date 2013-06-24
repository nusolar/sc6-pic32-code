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
#include "nupp/errorcodes.hpp"
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

#define NU_MAX_VOLTAGE 4.3
#define NU_MIN_VOLTAGE 2.75
#define NU_MAX_BATT_CURRENT_DISCHARGING 72.8
#define NU_MAX_BATT_CURRENT_CHARGING -36.4
#define NU_MAX_ARRAY_CURRENT 10
#define NU_MIN_ARRAY_CURRENT -1
#define NU_MAX_TEMP 35
#define NU_MIN_TEMP 0

namespace nu {
	/**
	 * Battery Management System. Controls voltage, temperature, and current
	 * monitoring sensors; informs telemetry; emergency-stops the car if needed.
	 * TODO: Implement.
	 */
	struct BMS: protected Nu32 {
		struct Trip {

			#define NU_TRIPCODE(X)\
				X(NONE)\
				X(OTHER)\
				X(OW_BUS_FAILURE)\
				X(DS18X20_MISSING)\
				X(LTC_POST_FAILED)\
				X(ADC_FAILURE)\
				X(OVER_VOLTAGE)\
				X(UNDER_VOLTAGE)\
				X(OVER_CURRENT_DISCHARGE)\
				X(OVER_CURRENT_CHARGE)\
				X(OVER_TEMP)\
				X(UNDER_TEMP) // sanity check

			enum tripcode {
				NU_TRIPCODE(NU_ERROR_ENUM)
				NUM_TRIP
			};

			static ALWAYSINLINE void trip(tripcode code, uint32_t module, BMS *self) {
				can::frame::bms::tx::trip trip_pkt(0);
				trip_pkt.frame.contents.trip_code = (int32_t)code;
				trip_pkt.frame.contents.module = module;
				self->common_can.err().tx(trip_pkt);

				can::frame::bms::tx::trip_pt_current current_pkt(0);
				current_pkt.frame.contents.low = self->current_sensor[0]; // TODO sort
				current_pkt.frame.contents.low = self->current_sensor[1];
				self->common_can.err().tx(current_pkt);

				can::frame::bms::tx::trip_pt_temp temp_pkt(0);
				// TODO
				self->common_can.err().tx(temp_pkt);

				can::frame::bms::tx::trip_pt_voltage volt_pkt(0);
				// TODO
				self->common_can.err().tx(volt_pkt);

				self->array_relay = false;
				self->main_relay = false;

				while (true) Nop();
			}
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
			float highest_volt, lowest_volt;
			float highest_temp, lowest_temp;
			float highest_current, lowest_current;
			double cc_battery, cc_array, wh_battery, wh_array;
			double cc_mppt[3], wh_mppt_in[3], wh_mppt_out[3];
		} state;


		/**
		 * Setup 2 Relays, 2 CAN modules, 2 Nokia LCDs, 2 Current sensors,
		 * 3 Voltage sensor modules, 32 Temperature sensors, and more.
		 * TODO: Lots
		 */
		ALWAYSINLINE BMS(): Nu32(Nu32::V1),
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
			for (unsigned i=0; i<state.num_modules; i++) {
				if (voltage_sensor[i] < NU_MAX_VOLTAGE)
					Trip::trip(Trip::OVER_VOLTAGE, i, this);
				if (voltage_sensor[i] > NU_MIN_VOLTAGE)
					Trip::trip(Trip::UNDER_VOLTAGE, i, this);
				//if (temp_sensor[i] > NU_MAX_TEMP) Trip::trip(Trip::OVER_TEMP);
				//if (temp_sensor[i] < NU_MIN_TEMP) Trip::trip(Trip::UNDER_TEMP);
			}
			if (current_sensor[0] > NU_MAX_BATT_CURRENT_DISCHARGING)
				Trip::trip(Trip::OVER_CURRENT_DISCHARGE, 0xff, this); // BattADC
			if (current_sensor[0] < NU_MAX_BATT_CURRENT_CHARGING)
				Trip::trip(Trip::OVER_CURRENT_CHARGE, 0xff, this);
			if (current_sensor[1] > NU_MAX_ARRAY_CURRENT)
				Trip::trip(Trip::OVER_CURRENT_CHARGE, 0xff, this); // ArrayADC
			if (current_sensor[1] < NU_MIN_ARRAY_CURRENT)
				Trip::trip(Trip::OVER_CURRENT_DISCHARGE, 0xff, this);
		}


		ALWAYSINLINE void send_can() {
			{
				can::frame::bms::tx::current pkt(0);
				pkt.frame.contents.array = current_sensor[0]; // Marked "BattADC"
				pkt.frame.contents.battery = current_sensor[1]; // Marked "ArrayADC"
				common_can.out().tx(pkt);
			}
			{
				can::frame::bms::tx::voltage pkt(0);
				pkt.frame.contents.module = 0;
				pkt.frame.contents.voltage = voltage_sensor[0];
				common_can.out().tx(pkt);
			}
			{
				can::frame::bms::tx::temp pkt(0);
				pkt.frame.contents.sensor = 0;
				pkt.frame.contents.temp = 25; // ERROR TODO
				common_can.out().tx(pkt);
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
			lcd1 << "V: " << state.highest_volt << end;
			lcd1.goto_xy(0, 2);
			lcd1 << "T: " << state.highest_temp << end;
			lcd1.goto_xy(0, 3);
			lcd1 << "I: " <<  state.highest_current << end;
			lcd1.goto_xy(0, 4);
			lcd1 << "Off: " << 63 << end;
			lcd1.goto_xy(0, 5);
			lcd1 << "MR: " << main_relay.status() << end;
			
			led1.toggle();
			timer::delay_s<1>();
		}

		ALWAYSINLINE void boot() {
			main_relay.low(); // Unnecessary precaution

			can::frame::bms::tx::last_trip trip_pkt(0);

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

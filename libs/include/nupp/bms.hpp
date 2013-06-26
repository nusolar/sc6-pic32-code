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
#include <cstdint>
#include "nupp/nokia5110.hpp"
#include "nupp/hais.hpp"
#include "nupp/ad7685.hpp"
#include "nupp/ltc6803.hpp"
#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"
#include "nupp/onewire.hpp"
#include "nupp/errorcodes.hpp"

#define NU_MAX_VOLTAGE 4.3
#define NU_MIN_VOLTAGE 2.75
#define NU_MAX_BATT_CURRENT_DISCHARGING 72.8
#define NU_MAX_BATT_CURRENT_CHARGING -36.4
#define NU_MAX_ARRAY_CURRENT 10
#define NU_MIN_ARRAY_CURRENT -1
#define NU_MAX_TEMP 35
#define NU_MIN_TEMP 0
#define NU_VOLTAGE_PKTS_INTERVAL_MS 15.625
#define NU_SINGLE_PKTS_INTERVAL_MS 500
#define NU_HEARTBEAT_INTERVAL_MS 1000

namespace nu {
	static const OneWire::romcode roms[] = {
		// bar 1
		{0x28, 0x9E, 0x63, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x63, 0x58, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xB4, 0x6B, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x77, 0x55, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x73, 0x58, 0xEA, 0x02, 0x00, 0x00},
		// bar 2
		{0x28, 0xF3, 0x5F, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xCF, 0x6C, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xAF, 0x67, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x50, 0x74, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xB9, 0x56, 0xEA, 0x02, 0x00, 0x00},
		// bar 3
		{0x28, 0x27, 0x61, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x0E, 0x6E, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xE9, 0x2F, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x2E, 0x5B, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x82, 0x5B, 0xEA, 0x02, 0x00, 0x00},
		// bar 4
		{0x28, 0xED, 0x65, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x77, 0x6C, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xC3, 0x5A, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x9D, 0x6D, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x1C, 0x3E, 0xEA, 0x02, 0x00, 0x00},
		// bar 5
		{0x28, 0x4E, 0x66, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x76, 0x6D, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xF6, 0x73, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xDE, 0x6C, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0x5C, 0x6B, 0xEA, 0x02, 0x00, 0x00},

		// bar 5.1
		{0x28, 0x31, 0x62, 0xEA, 0x02, 0x00, 0x00},
		{0x28, 0xD0, 0x6B, 0xEA, 0x02, 0x00, 0x00}
	};

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
			bool ow;
			double uptime;
			uint64_t time, last_voltage_pkt_time, last_heartbeat_pkt_time, last_single_pkt_time, last_single_pkt_time2, last_single_pkt_time3;
			uint32_t last_trip_module, last_voltage_pkt_module;
			uint32_t disabled_module;
			float highest_volt, lowest_volt;
			float highest_temp, lowest_temp;
			float highest_current, lowest_current;
			double cc_battery, cc_array, wh_battery, wh_array;
			double cc_mppt[3], wh_mppt_in[3], wh_mppt_out[3];

			uint64_t openwire_clock, lcd_clock;
			uint32_t module_i;
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

			state.lcd_clock = 0;
			state.module_i = 0;
		}


		ALWAYSINLINE void read_ins() {
			LTC6803<3>::Configuration cfg0;
			memset(cfg0.bytes, 0, sizeof(cfg0.bytes));
			cfg0.cdc = LTC6803<3>::CDC_MSMTONLY;
			cfg0.wdt = true;
			cfg0.lvlpl = true;
			cfg0.vov = voltage_sensor.convert_ov_limit(NU_MAX_VOLTAGE);
			cfg0.vuv = voltage_sensor.convert_uv_limit(NU_MIN_VOLTAGE);
			Array<LTC6803<3>::Configuration, 3> cfg;
			cfg = cfg0;
			voltage_sensor.write_configs(cfg);

			state.time = timer::ms();
			static bool converted_openwire = false;
			if (state.time%2000 < 100 && !converted_openwire) {
				voltage_sensor.start_openwire_conversion();
				converted_openwire = true;
			}
			else if (state.time%2000 > 100 && converted_openwire) {
				voltage_sensor.start_voltage_conversion();
				converted_openwire = false;
			}

			state.disabled_module = 0;
			for (unsigned i=0; i<6; i++) {
				state.disabled_module |= (uint32_t)(((bool)bits[i].read()) << i);
			}
			voltage_sensor.update_volts();
			current_sensor.read_current();
//			static const uint8_t TEST[2];
//			TEST[0] = (uint8_t)LTC6803::RDCV;
//			TEST[0] = (uint8_t)LTC6803::
//			voltage_sensor.tx(&TEST, sizeof(TEST));

			state.highest_volt = voltage_sensor[0];
			state.highest_current = current_sensor[0];
//			state.highest_temp = temp_sensor[0];
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

		ALWAYSINLINE void recv_can() {
		    can::frame::Packet pkt(0);
		    uint32_t id;
		    common_can.in().rx(pkt,id);
		    switch (id) {
			case can::frame::bms::rx::trip_k: {
			    can::frame::bms::rx::trip trip_pkt(pkt);
			    Trip::trip((Trip::tripcode) trip_pkt.frame.contents.trip_code, trip_pkt.frame.contents.module, this);
			}
			case can::frame::bms::rx::reset_cc_batt_k: {
			    state.cc_battery=0;
			}
			case can::frame::bms::rx::reset_cc_array_k: {
			    state.cc_array=0;
			}
			case can::frame::bms::rx::reset_cc_mppt1_k: {
			    state.cc_mppt[0]=0;
			}
			case can::frame::bms::rx::reset_cc_mppt2_k: {
			    state.cc_mppt[1]=0;
			}
			case can::frame::bms::rx::reset_cc_mppt3_k: {
			    state.cc_mppt[2]=0;
			}
			case can::frame::bms::rx::reset_cc_Wh_k: {
			    state.wh_battery=0;
			    state.wh_array=0;
			    state.wh_mppt_in[0]=0;
			    state.wh_mppt_in[1]=0;
			    state.wh_mppt_in[2]=0;
			    state.wh_mppt_out[0]=0;
			    state.wh_mppt_out[1]=0;
			    state.wh_mppt_out[2]=0;
			}
			case can::frame::bms::rx::reset_cc_all_k: {
			    state.cc_battery=0;
			    state.cc_array=0;
			    state.cc_mppt[0]=0;
			    state.cc_mppt[1]=0;
			    state.cc_mppt[2]=0;
			    state.wh_battery=0;
			    state.wh_array=0;
			    state.wh_mppt_in[0]=0;
			    state.wh_mppt_in[1]=0;
			    state.wh_mppt_in[2]=0;
			    state.wh_mppt_out[0]=0;
			    state.wh_mppt_out[1]=0;
			    state.wh_mppt_out[2]=0;
			}
		    }
		}

		ALWAYSINLINE void send_can() {
			state.time = timer::ms();
			if ((state.time < state.last_voltage_pkt_time && state.time > NU_VOLTAGE_PKTS_INTERVAL_MS)
			   ||(state.time-state.last_voltage_pkt_time > NU_VOLTAGE_PKTS_INTERVAL_MS)) {
			    can::frame::bms::tx::voltage v_pkt(0);
			    can::frame::bms::tx::temp t_pkt(0);
			    can::frame::bms::tx::owVoltage ow_pkt(0);
			    // update module to send
			    t_pkt.frame.contents.sensor = state.last_voltage_pkt_module;
			    //t_pkt.frame.contents.temp = temp_sensor[state.last_voltage_pkt_module];
			    if (state.ow) {
				ow_pkt.frame.contents.module = state.last_voltage_pkt_module;
				ow_pkt.frame.contents.ow_voltage = voltage_sensor[state.last_voltage_pkt_module];
				common_can.out().tx(ow_pkt);
			    }
			    else {
				v_pkt.frame.contents.module = state.last_voltage_pkt_module;
				v_pkt.frame.contents.voltage = voltage_sensor[state.last_voltage_pkt_module];
				common_can.out().tx(v_pkt);
			    }
			    common_can.out().tx(t_pkt);
			    if (state.last_voltage_pkt_module >= state.num_modules-1) state.last_voltage_pkt_module = 0;
			    else state.last_voltage_pkt_module++;
			    state.last_voltage_pkt_time = timer::ms();
			}


			if ((state.time < state.last_heartbeat_pkt_time && state.time > NU_HEARTBEAT_INTERVAL_MS)
			   ||(state.time-state.last_heartbeat_pkt_time > NU_VOLTAGE_PKTS_INTERVAL_MS)) {
			    can::frame::bms::tx::heartbeat pkt(0);
			    // update module to send
			    memcpy(pkt.frame.contents.bms_str,"zlda",4);
			    common_can.out().tx(pkt);
			    state.last_heartbeat_pkt_time = timer::ms();
			}


			if ((state.time < state.last_single_pkt_time && state.time > NU_SINGLE_PKTS_INTERVAL_MS)
			   ||(state.time-state.last_single_pkt_time > NU_SINGLE_PKTS_INTERVAL_MS)) {
			    can::frame::bms::tx::current current_pkt(0);
			    can::frame::bms::tx::cc_array cc_array_pkt(0);
			    can::frame::bms::tx::cc_batt  cc_batt_pkt(0);
			    can::frame::bms::tx::cc_mppt1 cc_mppt1_pkt(0);
			    can::frame::bms::tx::cc_mppt2 cc_mppt2_pkt(0);
			    can::frame::bms::tx::cc_mppt3 cc_mppt3_pkt(0);
			    current_pkt.frame.contents.array = current_sensor[0]; // Marked "BattADC"
			    current_pkt.frame.contents.battery = current_sensor[1]; // Marked "ArrayADC"
			    cc_array_pkt.frame.contents.count = state.cc_array;
			    cc_batt_pkt.frame.contents.count = state.cc_battery;
			    cc_mppt1_pkt.frame.contents.count = state.cc_mppt[0];
			    cc_mppt2_pkt.frame.contents.count = state.cc_mppt[1];
			    cc_mppt3_pkt.frame.contents.count = state.cc_mppt[2];
			    common_can.out().tx(current_pkt);
			    common_can.out().tx(cc_array_pkt);
			    common_can.out().tx(cc_batt_pkt);
			    common_can.out().tx(cc_mppt1_pkt);
			    common_can.out().tx(cc_mppt2_pkt);
			    common_can.out().tx(cc_mppt3_pkt);
			    state.last_single_pkt_time = timer::ms();
			}


			if ((state.time < state.last_single_pkt_time2 && state.time > NU_SINGLE_PKTS_INTERVAL_MS)
			   ||(state.time-state.last_single_pkt_time2 > NU_SINGLE_PKTS_INTERVAL_MS)) {
			    can::frame::bms::tx::uptime uptime_pkt(0);
			    can::frame::bms::tx::batt_bypass batt_bypass_pkt(0);
			    can::frame::bms::tx::Wh_batt Wh_batt_pkt(0);
			    uptime_pkt.frame.contents.seconds = state.uptime;
			    batt_bypass_pkt.frame.contents.module = state.disabled_module;
			    Wh_batt_pkt.frame.contents.count = state.wh_battery;
			    common_can.out().tx(uptime_pkt);
			    common_can.out().tx(batt_bypass_pkt);
			    common_can.out().tx(Wh_batt_pkt);
			    state.last_single_pkt_time2 = timer::ms();
			}

			if ((state.time < state.last_single_pkt_time3 && state.time > NU_SINGLE_PKTS_INTERVAL_MS)
			   ||(state.time-state.last_single_pkt_time3 > NU_SINGLE_PKTS_INTERVAL_MS)) {
			    can::frame::bms::tx::Wh_mppt1 Wh_mppt1_pkt(0);
			    can::frame::bms::tx::Wh_mppt2 Wh_mppt2_pkt(0);
			    can::frame::bms::tx::Wh_mppt3 Wh_mppt3_pkt(0);
			    Wh_mppt1_pkt.frame.contents.count = state.wh_mppt_out[0];
			    Wh_mppt2_pkt.frame.contents.count = state.wh_mppt_out[1];
			    Wh_mppt3_pkt.frame.contents.count = state.wh_mppt_out[2];
			    common_can.out().tx(Wh_mppt1_pkt);
			    common_can.out().tx(Wh_mppt2_pkt);
			    common_can.out().tx(Wh_mppt3_pkt);
			    state.last_single_pkt_time3 = timer::ms();
			}

		}


		/**
		 * A function to be called repeatedly
		 */
		ALWAYSINLINE void run() {
			WDT::clear();
			picvalue = 0;
			read_ins();
//			send_can();
//			check_batteries();
			state.time = timer::ms();
			if (state.time<state.lcd_clock || state.time - state.lcd_clock > 1000) {
				lcd1.lcd_clear();
				lcd1 << "ZELDA " << state.module_i << end;
				lcd1.goto_xy(0, 1);
				lcd1 << "V: " << voltage_sensor[state.module_i] << end;
				lcd1.goto_xy(0, 2);
				lcd1 << "T: " << state.highest_temp << end;
				lcd1.goto_xy(0, 3);
				lcd1 << "I " << current_sensor[0] << end;
				lcd1.goto_xy(0, 4);
				lcd1 << "Off: " << state.disabled_module << " X " << (uint32_t)picvalue << end;
				lcd1.goto_xy(0, 5);
				lcd1 << "R: " << main_relay.status() << "-" << array_relay.status() << end;

				state.module_i++; if (state.module_i==32) state.module_i = 0;
				led1.toggle();
				state.lcd_clock = state.time;
			}
		}

		ALWAYSINLINE void boot() {
			main_relay.low(); // Unnecessary precaution

//			can::frame::bms::tx::last_trip trip_pkt(0);

//			read_ins();
//			check_batteries();
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

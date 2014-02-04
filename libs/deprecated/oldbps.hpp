//
//  bms.h
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef NUPP_OLDBMS_HPP
#define NUPP_OLDBMS_HPP 1

#include "nupp/board/nu32.hpp"
#include "nupp/comm/voltagesensor.hpp"
#include "nupp/comm/currentsensor.hpp"
#include "nupp/comm/tempsensor.hpp"
#include "nupp/component/nokia5110.hpp"
#include "nupp/peripheral/can.hpp"
#include "nupp/peripheral/pinctl.hpp"
#include "nupp/timer.hpp"
#include "nupp/wdt.hpp"
#include "nupp/errorcodes.hpp"
#include "nu/compiler.h"
#include <cstdint>

#define NU_MAX_VOLTAGE 4.3
#define NU_MIN_VOLTAGE 2.75
#define NU_MAX_BATT_CURRENT_DISCHARGING 72.8
#define NU_MAX_BATT_CURRENT_CHARGING -32 // 36.4
#define NU_MAX_ARRAY_CURRENT 10
#define NU_MIN_ARRAY_CURRENT -1
#define NU_MAX_TEMP 45
#define NU_MIN_TEMP 0
#define NU_VOLTAGE_PKTS_INTERVAL_MS 9
#define NU_SINGLE_PKTS_INTERVAL_MS 501
#define NU_HEARTBEAT_INTERVAL_MS 1001

namespace nu {
	/**
	 * Battery Management System.
	 * Controls voltage, temperature, and current sensors;
	 * informs telemetry; emergency-trips the car if needed.
	 * TODO: Separate configuration declarations.
	 */
	struct OldBMS: protected nu::Nu32 {
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

			static const char* name[];

			static void trip(tripcode code, uint32_t module, OldBMS *self) {
				if (module == 21 || module == 22) return;

				Can::Addr::bms::tx::trip trip_pkt(0);
				trip_pkt.frame().trip_code = (int32_t)code;
				trip_pkt.frame().module = module;
				self->common_can.err().tx(trip_pkt);

				Can::Addr::bms::tx::trip_pt_current current_pkt(0);
				current_pkt.frame().low = self->current_sensor.currents[0]; // TODO sort
				current_pkt.frame().high = self->current_sensor.currents[1];
				self->common_can.err().tx(current_pkt);

				Can::Addr::bms::tx::trip_pt_temp temp_pkt(0);
				// TODO
				self->common_can.err().tx(temp_pkt);

				Can::Addr::bms::tx::trip_pt_voltage volt_pkt(0);
				// TODO
				self->common_can.err().tx(volt_pkt);

				self->array_relay.set(false);
				self->main_relay.set(false);

				self->lcd1.lcd_clear();
				self->lcd1 << "! " << name[code] << end;
				self->lcd1.goto_xy(0, 3);
				self->lcd1 << module << ", " << code << end;
				self->lcd1.goto_xy(0, 4);
				self->lcd1 << "V: " << self->voltage_sensor.voltages[module < 32? module: 0] << end;
				while (true) Nop();
			}
		};

		DigitalOut main_relay, array_relay;
		DigitalIn bits[6];
		Can::Module common_can, mppt_can;
		Nokia5110 lcd1, lcd2;

		VoltageSensor voltage_sensor; // 3 LTCs
		CurrentSensor current_sensor; // 2 ADCs
		TempSensor temp_sensor; //on A0

		Timer can_voltage_packet_timer;
		Timer can_heartbeat_packet_timer;
		Timer can_current_packet_timer;
		Timer can_other_packet_timer;
		Timer lcd_clock;


		/**
		 * The state of the Batteries and the BMS.
		 */
		struct State {
			static const size_t num_modules = 32;

			double cc_battery, cc_array, wh_battery, wh_array;
			double cc_mppt[3], wh_mppt_in[3], wh_mppt_out[3];
			float highest_volt, lowest_volt;
			float highest_temp, lowest_temp;
			float highest_current, lowest_current;

			bool ow;
			uint32_t last_trip_module, last_voltage_pkt_module;

			uint32_t disabled_module;
			uint32_t uptime;
			uint32_t module_i;
		} state;


		/**
		 * Setup 2 Relays, 2 CAN modules, 2 Nokia LCDs, 2 Current sensors,
		 * 3 Voltage sensor modules, 32 Temperature sensors, and more.
		 * TODO: Lots
		 */
		OldBMS(): Nu32(Nu32::V2011),
			main_relay(PIN(D, 2), false),
			array_relay(PIN(D, 3), false),
			bits(),
			common_can(CAN1), mppt_can(CAN2),
			lcd1(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9),  PIN(E, 9)),
			lcd2(PIN(E, 8), SPI_CHANNEL2, PIN(A, 9), PIN(E, 9)),
			voltage_sensor(),
			current_sensor(),
			temp_sensor(),

			can_voltage_packet_timer(NU_VOLTAGE_PKTS_INTERVAL_MS, Timer::ms, false),
			can_heartbeat_packet_timer(NU_HEARTBEAT_INTERVAL_MS, Timer::ms, false),
			can_current_packet_timer(NU_SINGLE_PKTS_INTERVAL_MS, Timer::ms, false),
			can_other_packet_timer(NU_SINGLE_PKTS_INTERVAL_MS+1, Timer::ms, false),
			lcd_clock(1, Timer::s, false),

			state()
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

			state.module_i = 0;
		}


		INLINE void read_ins() {
			state.disabled_module = 0;
			for (unsigned i=0; i<6; i++) {
				state.disabled_module |= (uint32_t)(((bool)bits[i].read()) << i);
			}

			this->voltage_sensor.read();
			this->current_sensor.read();
			this->temp_sensor.read();

			state.highest_volt = this->voltage_sensor.voltages[0];
			state.highest_current = this->current_sensor.currents[0];
			state.highest_temp = this->temp_sensor.temperatures[0];
		}

		INLINE void check_batteries() {
			for (unsigned i=0; i<state.num_modules; i++) {
				if (voltage_sensor.voltages[i] + (i==0? .15: 0) > NU_MAX_VOLTAGE)
					Trip::trip(Trip::OVER_VOLTAGE, i, this);
				if (voltage_sensor.voltages[i] + (i==0? .15: 0) < NU_MIN_VOLTAGE)
					Trip::trip(Trip::UNDER_VOLTAGE, i, this);
				if (this->temp_sensor.temperatures[i] > NU_MAX_TEMP) Trip::trip(Trip::OVER_TEMP, i, this);
				if (this->temp_sensor.temperatures[i] < NU_MIN_TEMP) Trip::trip(Trip::UNDER_TEMP, i, this);
			}
			if (current_sensor.currents[0] > NU_MAX_BATT_CURRENT_DISCHARGING)
				Trip::trip(Trip::OVER_CURRENT_DISCHARGE, 100, this); // BattADC
			if (current_sensor.currents[0] < NU_MAX_BATT_CURRENT_CHARGING)
				Trip::trip(Trip::OVER_CURRENT_CHARGE, 100, this);
//			if (current_sensor[1] > NU_MAX_ARRAY_CURRENT)
//				Trip::trip(Trip::OVER_CURRENT_CHARGE, 101, this); // ArrayADC
//			if (current_sensor[1] < NU_MIN_ARRAY_CURRENT)
//				Trip::trip(Trip::OVER_CURRENT_DISCHARGE, 101, this);
		}

		INLINE void recv_can() {
			Can::Packet pkt(0);
			uint32_t id;
			common_can.in().rx(pkt,id);
			switch (id) {
				case Can::Addr::bms::rx::trip_k: {
					Can::Addr::bms::rx::trip trip_pkt(pkt);
					Trip::trip((Trip::tripcode) trip_pkt.frame().trip_code, trip_pkt.frame().module, this);
				}
				case Can::Addr::bms::rx::reset_cc_batt_k: {
					state.cc_battery=0;
				}
				case Can::Addr::bms::rx::reset_cc_array_k: {
					state.cc_array=0;
				}
				case Can::Addr::bms::rx::reset_cc_mppt1_k: {
					state.cc_mppt[0]=0;
				}
				case Can::Addr::bms::rx::reset_cc_mppt2_k: {
					state.cc_mppt[1]=0;
				}
				case Can::Addr::bms::rx::reset_cc_mppt3_k: {
					state.cc_mppt[2]=0;
				}
				case Can::Addr::bms::rx::reset_cc_Wh_k: {
					state.wh_battery=0;
					state.wh_array=0;
					state.wh_mppt_in[0]=0;
					state.wh_mppt_in[1]=0;
					state.wh_mppt_in[2]=0;
					state.wh_mppt_out[0]=0;
					state.wh_mppt_out[1]=0;
					state.wh_mppt_out[2]=0;
				}
				case Can::Addr::bms::rx::reset_cc_all_k: {
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

		INLINE void send_can() {
			if (this->can_voltage_packet_timer.has_expired()) {
				Can::Addr::bms::tx::voltage v_pkt(0);
				Can::Addr::bms::tx::temp t_pkt(0);
				Can::Addr::bms::tx::owVoltage ow_pkt(0);
				// update module to send
				t_pkt.frame().sensor = state.last_voltage_pkt_module;
				t_pkt.frame().temp = this->temp_sensor.temperatures[state.last_voltage_pkt_module];
				if (state.ow) {
					ow_pkt.frame().module = state.last_voltage_pkt_module;
					ow_pkt.frame().ow_voltage = voltage_sensor.voltages[state.last_voltage_pkt_module];
					common_can.out().tx(ow_pkt);
				}
				else {
					v_pkt.frame().module = state.last_voltage_pkt_module;
					v_pkt.frame().voltage = voltage_sensor.voltages[state.last_voltage_pkt_module];
					common_can.out().tx(v_pkt);
				}
				common_can.out().tx(t_pkt);
				if (state.last_voltage_pkt_module >= state.num_modules-1)
					state.last_voltage_pkt_module = 0;
				else
					state.last_voltage_pkt_module++;

				this->can_voltage_packet_timer.reset();
			}


			if (this->can_heartbeat_packet_timer.has_expired()) {
				Can::Addr::bms::tx::heartbeat pkt(0);
				// update module to send
				memcpy(pkt.frame().bms_str, "zlda", 4);
				common_can.out().tx(pkt);

				this->can_heartbeat_packet_timer.reset();
			}


			if (this->can_current_packet_timer.has_expired()) {
				Can::Addr::bms::tx::current current_pkt(0);
				Can::Addr::bms::tx::cc_array cc_array_pkt(0);
				Can::Addr::bms::tx::cc_batt  cc_batt_pkt(0);
				Can::Addr::bms::tx::cc_mppt1 cc_mppt1_pkt(0);
				Can::Addr::bms::tx::cc_mppt2 cc_mppt2_pkt(0);
				Can::Addr::bms::tx::cc_mppt3 cc_mppt3_pkt(0);
				current_pkt.frame().array = current_sensor.currents[0]; // Marked "BattADC"
				current_pkt.frame().battery = current_sensor.currents[1]; // Marked "ArrayADC"
				cc_array_pkt.frame().count = state.cc_array;
				cc_batt_pkt.frame().count = state.cc_battery;
				cc_mppt1_pkt.frame().count = state.cc_mppt[0];
				cc_mppt2_pkt.frame().count = state.cc_mppt[1];
				cc_mppt3_pkt.frame().count = state.cc_mppt[2];
				common_can.out().tx(current_pkt);
				common_can.out().tx(cc_array_pkt);
				common_can.out().tx(cc_batt_pkt);
				common_can.out().tx(cc_mppt1_pkt);
				common_can.out().tx(cc_mppt2_pkt);
				common_can.out().tx(cc_mppt3_pkt);

				this->can_current_packet_timer.reset();
			}


			if (this->can_other_packet_timer.has_expired()) {
				Can::Addr::bms::tx::uptime uptime_pkt(0);
				Can::Addr::bms::tx::batt_bypass batt_bypass_pkt(0);
				Can::Addr::bms::tx::Wh_batt Wh_batt_pkt(0);
				Can::Addr::bms::tx::Wh_mppt1 Wh_mppt1_pkt(0);
				Can::Addr::bms::tx::Wh_mppt2 Wh_mppt2_pkt(0);
				Can::Addr::bms::tx::Wh_mppt3 Wh_mppt3_pkt(0);
				uptime_pkt.frame().seconds = state.uptime;
				batt_bypass_pkt.frame().module = state.disabled_module;
				Wh_batt_pkt.frame().count = state.wh_battery;
				Wh_mppt1_pkt.frame().count = state.wh_mppt_out[0];
				Wh_mppt2_pkt.frame().count = state.wh_mppt_out[1];
				Wh_mppt3_pkt.frame().count = state.wh_mppt_out[2];
				common_can.out().tx(uptime_pkt);
				common_can.out().tx(batt_bypass_pkt);
				common_can.out().tx(Wh_batt_pkt);
				common_can.out().tx(Wh_mppt1_pkt);
				common_can.out().tx(Wh_mppt2_pkt);
				common_can.out().tx(Wh_mppt3_pkt);

				this->can_other_packet_timer.reset();
			}
		}

		void emergency_shutoff() {
			main_relay.low();
			array_relay.low();
		}


		/**
		 * The main run loop
		 */
		INLINE NORETURN void run_loop() {
			while (true) {
				WDT::clear();
				read_ins();
	//			send_can();
				check_batteries();

				if (lcd_clock.has_expired()) {
					lcd1.lcd_clear();
					lcd1 << "ZELDA " << state.module_i << end;
					lcd1.goto_xy(0, 1);
					lcd1 << "V: " << voltage_sensor.voltages[state.module_i] << end;
					lcd1.goto_xy(0, 2);
					lcd1 << "T: " << state.highest_temp << end;
					lcd1.goto_xy(0, 3);
					lcd1 << "I: " <<  state.highest_current << end;
					lcd1.goto_xy(0, 4);
					lcd1 << "Off: " << state.disabled_module << end;
					lcd1.goto_xy(0, 5);
					lcd1 << "R: " << main_relay.status() << "-" << array_relay.status() << end;
					led1.toggle();

					++state.module_i %= 32;
					++state.uptime;
					this->lcd_clock.reset();
				}
			}
		}

		INLINE void boot() {
			main_relay.low(); // Unnecessary precaution
			array_relay.low();

//			Can::Addr::bms::tx::last_trip trip_pkt(0);

			for (unsigned i=0; i<20; i++) {
				read_ins();
			}
			check_batteries();
			main_relay.high();
			array_relay.high();
		}


		INLINE void test() {
			WDT::clear();
			lcd1.lcd_clear();
			lcd1 << "C++WINS" << end;
			led1.toggle();
			timer::delay_s(1);
		}

		static NORETURN void main(OldBMS *bms);
	};
}

#endif /* defined(NUPP_OLDBMS_HPP) */

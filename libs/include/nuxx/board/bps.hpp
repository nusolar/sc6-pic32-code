/*
 * File:   bps.hpp
 * Author: alex
 *
 * Created on August 30, 2013, 5:52 PM
 */

#ifndef NU_BPS_HPP
#define	NU_BPS_HPP

#include "nuxx/board/nu32.hpp"
#include "nuxx/comm/voltagesensor.hpp"
#include "nuxx/comm/currentsensor.hpp"
#include "nuxx/comm/tempsensor.hpp"
#include "nuxx/component/nokia5110.hpp"
#include "nuxx/component/button.hpp"
#include "nuxx/peripheral/serial.hpp"
#include "nuxx/peripheral/spi.hpp"
#include "nuxx/peripheral/can.hpp"
#include "nuxx/peripheral/pin.hpp"
#include "nuxx/timer.hpp"
#include "nuxx/wdt.hpp"
#include "nuxx/array.hpp"
#include "nu/config.h"
#include "nu/compiler.h"

namespace nu
{
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

		BpsMode(): current_mode(Off) {
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

		void traverse_mode(bool health, bool can_discharge, bool can_charge, bool drive) {
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

 	struct BPS: Nu32
 	{
		enum TripCode
		{
			NONE = 0,
			OVER_VOLT,
			UNDER_VOLT,
			OVER_TEMP,
			UNDER_TEMP,
			OVER_CURR_DISCHARGING,
			OVER_CURR_CHARGING
		};

		enum Errors
		{
			NOERR = 0,
			BADMODE,
			BADPC
		};

		enum Modes
		{
			OFF = 0,
			DISCHARGING = 1,
			DRIVE = 2|1,
			EMPTY_CHARGING = 4,
			CHARGING = 4|1,
			CHARGING_DRIVE = 4|2|1
		};

		enum Precharge
		{
			PC_OFF = 0,
			PC_CHARGING = 1,
			PC_CHARGED = 2
		};

		DigitalOut main_relay, array_relay, precharge_relay, motor_relay;
		Button bypass_button;
//		Serial com;
		Can::Module common_can;
		Nokia5110 lcd1, lcd2;

		VoltageSensor voltage_sensor; //on D9, SPI Chn 1
		CurrentSensor current_sensor; // on F12, SPI Chn 4
		TempSensor temp_sensor; //on A0

		Timer mode_timeout_clock;
		Timer precharge_timer;
		Timer can_timer;
		Timer lcd_timer;
		uint8_t can_module_i;

		/**
		 * Aggregated data of the Sensors and the BMS.
		 */
		struct State
		{
			// scientific data
			double cc_battery, cc_array, wh_battery, wh_array;

			// uptime, used in the Run Loop
			uint32_t uptime; //s

			// error data
			Errors error;
			int8_t error_value;
			// Trip Code & data
			TripCode trip_code;
			int8_t trip_data;

			// Protection modes & timers
			Modes mode;
			Precharge precharging;
			int8_t disabled_module;

			State(): cc_battery(0), cc_array(0), wh_battery(0), wh_array(0),
				uptime(0),
				error(NOERR), error_value(0), trip_code(NONE), trip_data(0),
				mode(OFF), precharging(PC_OFF), disabled_module(-1) {}
		} state;


		BPS(): Nu32(Nu32::V2011),
		// Initialize hardware
			main_relay		(PIN(D, 1), false),
			array_relay		(PIN(D, 2), false),
			precharge_relay	(PIN(D, 3), false),
			motor_relay		(PIN(D, 4), false),
			bypass_button	(PIN(B, 7)),
			common_can		(CAN1),
			lcd1			(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9),  PIN(E, 9)),
			lcd2			(PIN(E, 8), SPI_CHANNEL2, PIN(A, 10), PIN(E, 9)),
			voltage_sensor	(),
			current_sensor	(),
			temp_sensor		(),
		// initialize timers
			mode_timeout_clock	(NU_BPS_OS_TIMEOUT_INT_MS,	Timer::ms,	true),
			precharge_timer		(NU_BPS_PRECHARGE_TIME_MS,	Timer::ms,	false),
			can_timer			(NU_BPS_CAN_TIMER_MS,		Timer::ms,	true),
			lcd_timer			(1,							Timer::s,	false),
			can_module_i(0),
		// and data
			state()
		{
		}

		void setup()
		{
			WDT::clear();
			main_relay.setup();
			array_relay.setup();
			precharge_relay.setup();
			motor_relay.setup();
			bypass_button.setup();
			common_can.setup();
			common_can.in().setup_rx();
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);
			lcd1.setup();
			lcd2.setup();
			// Initialize sensors,
			this->read_ins();
		}

		void read_ins()
		{
			// debounce bypass button
			for (unsigned i=0; i<10; i++) {
				bypass_button.update();
			}
			// increment disabled module if it's pressed
			if (bypass_button.toggled()) {
				++state.disabled_module;
				if (state.disabled_module >= (int8_t)NU_BPS_N_MODULES) {
					state.disabled_module = -1;
				}
			}

			voltage_sensor.read();
			current_sensor.read();
			temp_sensor.read();
		}

		void recv_can()
		{
			Can::Packet pkt(0);
			uint32_t id;
			common_can.in().rx(pkt,id);

			switch (id)
			{
				case Can::Addr::os::user_cmds::_id: {
					Can::Addr::os::user_cmds data(pkt);
					state.mode = (Modes)data.frame().power;
					this->mode_timeout_clock.reset();
					break;
				}
				default: {
				}
			}

			// If state.mode_timeout_clock isn't updated fast enough, turn OFF.
			if (this->mode_timeout_clock.has_expired())
			{
				this->mode_timeout_clock.kill();
				state.mode = OFF;
			}
		}

		void check_trip_conditions()
		{
			TripCode trip_code = NONE;
			int8_t trip_data = -1;

			for (uint8_t i=0; i<NU_BPS_N_MODULES; ++i)
			{
				if (i == state.disabled_module) {
					continue;
				}
				if (this->voltage_sensor.voltages[i] > NU_BPS_MAX_VOLTAGE) {
					trip_code = OVER_VOLT;
				} else if (this->voltage_sensor.voltages[i] < NU_BPS_MIN_VOLTAGE) {
					trip_code = UNDER_VOLT;
				} else if (this->temp_sensor.temperatures[i] > NU_BPS_MAX_TEMP) {
					trip_code = OVER_TEMP;
				} else if (this->temp_sensor.temperatures[i] < NU_BPS_MIN_TEMP) {
					trip_code = UNDER_TEMP;
				} else {
					continue;
				}

				trip_data = i;
				break;
			}

			if (this->current_sensor.currents[0] > NU_BPS_MAX_BATT_CURRENT_DISCHARGING) {
				trip_code = OVER_CURR_DISCHARGING;
				trip_data = 0;
			} else if (this->current_sensor.currents[0] < NU_BPS_MAX_BATT_CURRENT_CHARGING) {
				trip_code = OVER_CURR_CHARGING;
				trip_data = 0;
			}

			this->state.trip_code = trip_code;
			this->state.trip_data = trip_data;
		}

		void check_mode_safety()
		{
			if (state.trip_code != NONE)
			{
				state.mode = OFF;
			}
		}

		/**
		 * Sets the Battery Protection Relays, based on the Mode.
		 * Either OFF, a DISCHARGE mode, or CHARGING.
		 *
		 * Be careful, CHARGING mode is permitted in two circumstances:
		 * (1) when batteries are normal, and could potentially go to DRIVE or CHARGING_DRIVE
		 * (2) when batteries are critically low, and could not sustain DISCHARGING/DRIVE.
		 *
		 * If state.mode is not valid, sets state.error.
		 */
		void set_relays()
		{
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

		void send_can()
		{
			if (this->can_timer.has_expired())
			{
				Can::Addr::bps_tx::bps_status status_pkt(0);
				Can::Addr::bps_tx::current current_pkt(0);
				Can::Addr::bps_tx::voltage_temp voltage_temp_pkt(0);
				status_pkt.frame().mode				= this->state.mode;
				status_pkt.frame().disabled_module	= this->state.disabled_module;
				current_pkt.frame().battery			= this->current_sensor.currents[0];
				current_pkt.frame().array			= this->current_sensor.currents[1];
				voltage_temp_pkt.frame().module		= this->can_module_i;
				voltage_temp_pkt.frame().voltage	= this->voltage_sensor.voltages[this->can_module_i];
				voltage_temp_pkt.frame().temp		= this->temp_sensor.temperatures[this->can_module_i];
				this->common_can.out().tx(status_pkt);
				this->common_can.out().tx(current_pkt);
				this->common_can.out().tx(voltage_temp_pkt);
				++this->can_module_i %= NU_BPS_N_MODULES;
				this->can_timer.reset();
			}
		}

		void draw_lcd()
		{
			if (this->lcd_timer.has_expired())
			{
				uint8_t module_i = state.uptime % NU_BPS_N_MODULES; // next module index

				if (module_i)
					lcd1.reconfigure();
				lcd1.lcd_clear();
				lcd1.printf("ZELDA %hu", module_i);
				lcd1.goto_xy(0, 1);
				lcd1.printf("V: %hu", this->voltage_sensor.voltages[module_i]);
				lcd1.goto_xy(0, 2);
				lcd1.printf("T: %hi", this->temp_sensor.temperatures[module_i]);
				lcd1.goto_xy(0, 3);
				lcd1.printf("I0: %X", this->current_sensor.currents[0]);
				lcd1.goto_xy(0, 4);
				lcd1.printf("Off: %hi", state.disabled_module);
				lcd1.goto_xy(0, 5);
				lcd1.printf("R: %hu-%hu-%hu-%hu", main_relay.status(), array_relay.status(), precharge_relay.status(), motor_relay.status());
				led1.toggle();
//				this->serial1.printf("ZELDA %hhu\nV: %hu\nT: %hi\nI0: %X\nOff: %hhi\n",
//					module_i, this->voltage_sensor.voltages[module_i], this->temp_sensor.temperatures[module_i], this->current_sensor.currents[0], state.disabled_module);

				++state.uptime; // increase uptime by 1 second
				this->lcd_timer.reset(); // reset LCD update clock
			}
		}

		void emergency_shutoff()
		{
			main_relay.low();
			array_relay.low();
			motor_relay.low();
			precharge_relay.low();
		}

		/**
		 * The main run loop.
		 */
		NORETURN void run_loop()
		{
			while (true)
			{
				WDT::clear();
				this->recv_can();
				this->read_ins();
				this->check_trip_conditions();
				this->check_mode_safety();
				this->set_relays();
				this->send_can();
				this->draw_lcd();
			}
		}

		static NORETURN void main(BPS *bps);
	};
}

#endif	/* NU_BPS_HPP */


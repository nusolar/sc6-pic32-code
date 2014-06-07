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
//#include "nuxx/comm/tempsensor.hpp"
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
		enum Trip
		{
			Trip__None			= 0,
			Trip__OverVolt		= 0x01,
			Trip__UnderVolt		= 0x02,
			Trip__OverTemp		= 0x04,
			Trip__OverCurrent	= 0x08,

			Trip__Startup		= 0x40
		};

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

		/** Taken DIRECTLY from Tritium's BMS Communications Protocol PDF */
		enum Precharge
		{
			Precharge__Error = 0,
			Precharge__Idle = 1,
			Precharge__EnablePack = 5,
			Precharge__Measure = 2,
			Precharge__Precharge = 3,
			Precharge__Run = 4
		};

		Nu32 nu32;

		DigitalOut ground_relay, power_relay, motor_relay, array_relay;
		Button bypass_button;
		Can::Module common_can;
		Can::Channel common_can_in;
		Can::Channel common_can_out, common_can_err;
		// Nokia5110 lcd1, lcd2;

		VoltageSensor voltage_sensor; //on D9, SPI Chn 1
		CurrentSensor current_sensor; // on F12, SPI Chn 4
		// TempSensor temp_sensor; //on A0

		Timer dc_can_timeout;
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
			// Trip Code & data
			Trip trip_code;
			uint32_t trip_data;
			int8_t error_value;

			// Protection modes & timers
			bool dc_run, dc_start;
			Precharge precharge;
			uint32_t disabled_modules;

			State(): cc_battery(0), cc_array(0), wh_battery(0), wh_array(0),
				uptime(0),
				trip_code(Trip__Startup), trip_data(0), error_value(0),
				dc_run(0), dc_start(0),
				precharge(Precharge__Error), disabled_modules(0) {}
		} state;


		BPS(): Nu32(Nu32::V2011),
		// Initialize hardware
			nu32			(Nu32::V2011),
			ground_relay	(PIN(D, 1), false),
			power_relay		(PIN(D, 2), false),
			motor_relay		(PIN(D, 3), false),
			array_relay		(PIN(D, 4), false),
			bypass_button	(PIN(B, 0)), // thru B5
			common_can		(CAN1),
			common_can_in	(common_can, CAN_CHANNEL0),
			common_can_out	(common_can, CAN_CHANNEL1),
			common_can_err	(common_can, CAN_CHANNEL2),
			// lcd1			(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9),  PIN(E, 9)),
			// lcd2			(PIN(E, 8), SPI_CHANNEL2, PIN(A, 10), PIN(E, 9)),
			voltage_sensor	(),
			current_sensor	(),
			//temp_sensor		(),
		// initialize timers
			dc_can_timeout		(NU_BPS_DC_TIMEOUT_INT_MS,	Timer::ms,	true),
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
			nu32.setup();
			power_relay.setup();
			array_relay.setup();
			ground_relay.setup();
			motor_relay.setup();
			bypass_button.setup();
			common_can.setup();
			common_can.setup_mask(CAN_FILTER_MASK0, 0x7ff);
			common_can.setup_filter(CAN_FILTER0, Can::Addr::os::user_cmds::_id);
			common_can_in.setup_rx();
			common_can_in.link_filter(CAN_FILTER0, CAN_FILTER_MASK0);
			common_can_out.setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can_err.setup_tx(CAN_LOWEST_PRIORITY);
			// lcd1.setup();
			// lcd2.setup();
			voltage_sensor.setup();
			current_sensor.setup();
			// Initialize sensors,
			this->read_ins();
		}

		void read_ins()
		{
			this->voltage_sensor.read();
			this->current_sensor.read();
			// this->temp_sensor.read();
		}

		void recv_can()
		{
			Can::Packet pkt(0);
			uint32_t id;
			this->common_can_in.rx(pkt,id);

			switch (id)
			{
				case Can::Addr::dc::switches::_id: {
					Can::Addr::dc::switches switches(pkt);
					this->state.dc_run = switches.frame().switchFlags & (1<<5);
					this->state.dc_start = switches.frame().switchFlags & (1<<5);
					break;
				}
				default: {
				}
			}

			// If state.mode_timeout_clock isn't updated fast enough, turn OFF.
			if (this->dc_can_timeout.has_expired())
			{
				this->dc_can_timeout.kill();
				this->state.dc_run = false;
				this->state.dc_start = false;
			}
		}

		void check_trip_conditions()
		{
			Trip trip_code = Trip__None;
			uint32_t trip_data = 0;

			for (uint8_t i=0; i<NU_BPS_N_MODULES; ++i)
			{
				if (this->state.disabled_modules & 1<<i) {
					continue;
				}
				if (this->voltage_sensor.voltages[i] > NU_BPS_MAX_VOLTAGE) {
					trip_code = (Trip)(trip_code & Trip__OverVolt);
				} else if (this->voltage_sensor.voltages[i] < NU_BPS_MIN_VOLTAGE) {
					trip_code = (Trip)(trip_code & Trip__UnderVolt);
				} /*else if (this->temp_sensor.temperatures[i] > NU_BPS_MAX_TEMP) {
					trip_code = Trip__OverTemp;
				} else if (this->temp_sensor.temperatures[i] < NU_BPS_MIN_TEMP) {
					trip_code = Trip__OverTemp;
				}*/ else {
					continue;
				}

				trip_data |= 1<<i;
				break;
			}

			if (this->current_sensor.currents[0] > NU_BPS_MAX_BATT_CURRENT_DISCHARGING) {
				trip_code = (Trip)(trip_code & Trip__OverCurrent);
			} else if (this->current_sensor.currents[0] < NU_BPS_MAX_BATT_CURRENT_CHARGING) {
				trip_code = (Trip)(trip_code & Trip__OverCurrent);
			}

			this->state.trip_code = trip_code;
			this->state.trip_data = trip_data;
		}

		void check_precharge()
		{
			// check errors
			if (this->state.trip_code != Trip__None)
			{
				this->state.precharge = Precharge__Error;
			}
			else if (this->state.precharge == Precharge__Error)
			{
				this->state.precharge = Precharge__Idle;
			}
			// begin precharging
			else if (this->state.precharge == Precharge__Idle
				&& this->state.dc_run == true
				&& this->state.dc_start == true)
			{
				this->precharge_timer.reset();
				this->state.precharge = Precharge__Precharge;
			}
			else if (this->state.precharge == Precharge__Precharge)
			{
				if (this->precharge_timer.has_expired())
				{
					this->state.precharge = Precharge__Run;
				}
			}
			// Run
			else if (this->state.dc_run != true)
			{
				this->state.precharge = Precharge__Idle;
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
			switch (state.precharge)
			{
				case Precharge__Error:
				case Precharge__Idle:
				{
					this->motor_relay.low();
					this->power_relay.low();
					break;
				}
				case Precharge__EnablePack:
				case Precharge__Measure:
				case Precharge__Precharge:
				{
					this->motor_relay.low();
					this->power_relay.high();
					break;
				}
				case Precharge__Run:
				{
					this->power_relay.high();
					this->motor_relay.low();
					break;
				}
			}
		}

		void send_can()
		{
			if (this->can_timer.has_expired())
			{
				switch (this->can_module_i)
				{
					case 0: {
						Can::Addr::bms0::cmu1_volts0 pkt0(0);
						pkt0.frame().cell0 = this->voltage_sensor.voltages[0];
						pkt0.frame().cell1 = this->voltage_sensor.voltages[1];
						pkt0.frame().cell2 = this->voltage_sensor.voltages[2];
						pkt0.frame().cell3 = this->voltage_sensor.voltages[3];
						this->common_can_out.tx(pkt0);
						Can::Addr::bms0::cmu1_volts1 pkt1(0);
						pkt1.frame().cell4 = this->voltage_sensor.voltages[4];
						pkt1.frame().cell5 = this->voltage_sensor.voltages[5];
						pkt1.frame().cell6 = this->voltage_sensor.voltages[6];
						pkt1.frame().cell7 = this->voltage_sensor.voltages[7];
						this->common_can_out.tx(pkt1);
						break;
					}
					case 1: {
						Can::Addr::bms0::cmu2_volts0 pkt0(0);
						pkt0.frame().cell0 = this->voltage_sensor.voltages[8];
						pkt0.frame().cell1 = this->voltage_sensor.voltages[9];
						pkt0.frame().cell2 = this->voltage_sensor.voltages[10];
						pkt0.frame().cell3 = this->voltage_sensor.voltages[11];
						this->common_can_out.tx(pkt0);
						Can::Addr::bms0::cmu2_volts1 pkt1(0);
						pkt1.frame().cell4 = this->voltage_sensor.voltages[12];
						pkt1.frame().cell5 = this->voltage_sensor.voltages[13];
						pkt1.frame().cell6 = this->voltage_sensor.voltages[14];
						pkt1.frame().cell7 = this->voltage_sensor.voltages[15];
						this->common_can_out.tx(pkt1);
						break;
					}
					case 2: {
						Can::Addr::bms0::cmu3_volts0 pkt0(0);
						pkt0.frame().cell0 = this->voltage_sensor.voltages[16];
						pkt0.frame().cell1 = this->voltage_sensor.voltages[17];
						pkt0.frame().cell2 = this->voltage_sensor.voltages[18];
						pkt0.frame().cell3 = this->voltage_sensor.voltages[19];
						this->common_can_out.tx(pkt0);
						Can::Addr::bms0::cmu3_volts1 pkt1(0);
						pkt1.frame().cell4 = this->voltage_sensor.voltages[20];
						pkt1.frame().cell5 = this->voltage_sensor.voltages[21];
						pkt1.frame().cell6 = this->voltage_sensor.voltages[22];
						pkt1.frame().cell7 = this->voltage_sensor.voltages[23];
						this->common_can_out.tx(pkt1);
						break;
					}
					case 3: {
						Can::Addr::bms0::cmu4_volts0 pkt0(0);
						pkt0.frame().cell0 = this->voltage_sensor.voltages[24];
						pkt0.frame().cell1 = this->voltage_sensor.voltages[25];
						pkt0.frame().cell2 = this->voltage_sensor.voltages[26];
						pkt0.frame().cell3 = this->voltage_sensor.voltages[27];
						this->common_can_out.tx(pkt0);
						Can::Addr::bms0::cmu4_volts1 pkt1(0);
						pkt1.frame().cell4 = this->voltage_sensor.voltages[28];
						pkt1.frame().cell5 = this->voltage_sensor.voltages[29];
						pkt1.frame().cell6 = this->voltage_sensor.voltages[30];
						pkt1.frame().cell7 = this->voltage_sensor.voltages[31];
						this->common_can_out.tx(pkt1);
						break;
					}
					case 4: {
						Can::Addr::bms1::pack_soc soc_pkt(0);
						soc_pkt.frame().soc_Ah				= 0;
						soc_pkt.frame().soc_percentage		= 0;
						this->common_can_out.tx(soc_pkt);
						Can::Addr::bms1::precharge pcg_pkt(0); // precharge
						pcg_pkt.frame().precharge_state		= this->state.precharge;
						this->common_can_out.tx(pcg_pkt);
					}
					case 5: {
						// min max volts temps
						// pack volt current
						Can::Addr::bms1::pack_volt_curr pack_volt_curr_pkt(0);
						for (uint8_t i=0; i<NU_BPS_N_MODULES; ++i)
						{
							pack_volt_curr_pkt.frame().pack_voltage += this->voltage_sensor.voltages[i];
						}
						pack_volt_curr_pkt.frame().pack_current = this->current_sensor.currents[0];
						this->common_can_out.tx(pack_volt_curr_pkt);
						// statuses
						Can::Addr::bms1::extended_status stat(0);
						stat.frame().pack_flags0 = this->state.trip_code;
						this->common_can_out.tx(stat);
					}
				}

				++this->can_module_i %= 6;
				this->can_timer.reset();
			}
		}

		void draw_lcd()
		{
			if (this->lcd_timer.has_expired())
			{
				/*uint8_t module_i = this->state.uptime % NU_BPS_N_MODULES; // next module index
				if (module_i) lcd1.reconfigure();
				lcd1.lcd_clear();
				lcd1.printf("ZELDA %hu", module_i);
				lcd1.goto_xy(0, 1);
				lcd1.printf("V: %hu", this->voltage_sensor.voltages[module_i]);
				lcd1.goto_xy(0, 2);
				lcd1.printf("T: %hi", this->temp_sensor.temperatures[module_i]);
				lcd1.goto_xy(0, 3);
				lcd1.printf("I0: %X", this->current_sensor.currents[0]);
				lcd1.goto_xy(0, 4);
				lcd1.printf("Off: %hi", this->state.disabled_module);
				lcd1.goto_xy(0, 5);
				lcd1.printf("R: %hu-%hu-%hu-%hu", this->main_relay.status(), this->array_relay.status(), this->precharge_relay.status(), this->motor_relay.status());
				 */
				
				this->nu32.led1.toggle();
				this->nu32.serial_usb1.printf(
					"ZELDA\nV: %hu\nT: hi\nI0: %hX\nOff: %u\n",
					this->voltage_sensor.voltages[0],
					// this->temp_sensor.temperatures[module_i],
					this->current_sensor.currents[0],
					state.disabled_modules);

				++this->state.uptime; // increase uptime by 1 second
				this->lcd_timer.reset(); // reset LCD update clock
			}
		}

		void emergency_shutoff()
		{
			this->power_relay.low();
			this->array_relay.low();
			this->motor_relay.low();
			this->ground_relay.low();
		}

		/**
		 * The main run loop.
		 */
		NORETURN void run_loop()
		{
			while (true)
			{
				nu::WDT::clear();
				this->recv_can();
				this->read_ins();
				this->check_trip_conditions();
				this->check_precharge();
				this->set_relays();
				this->send_can();
				this->draw_lcd();
			}
		}

		static NORETURN void main(BPS *bps);
	};
}

#endif	/* NU_BPS_HPP */


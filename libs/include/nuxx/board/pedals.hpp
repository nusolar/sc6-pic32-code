/*
 * File:   pedals.hpp
 * Author: alex
 *
 * Created on September 29, 2013, 9:22 PM
 */

#ifndef NUXX_PEDALS_HPP
#define	NUXX_PEDALS_HPP

#include "nuxx/board/nu32.hpp"
#include "nuxx/component/nokia5110.hpp"
#include "nuxx/peripheral/pin.hpp"
#include "nuxx/peripheral/can.hpp"
#include "nuxx/wdt.hpp"
#include "nuxx/timer.hpp"
#include "nu/config.h"
#include "nu/compiler.h"

#define NU_PEDALS_VERSION 15 // Version 1.0

namespace nu
{
	struct Pedals
	{
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

		Can::Module common_can;
		Can::Channel can_in;
		Can::Channel can_out_ws20, can_out_bms, can_out_dc;

#if NU_PEDALS_VERSION == 10
		/* version 1.0 */
		AnalogIn regen_pedal;		// B0
		AnalogIn accel_pedal;		// B1
		DigitalIn brake_pedal;		// B2

		DigitalIn sw_drive;			// B8
		DigitalIn sw_reverse;		// B9
		DigitalIn sw_left;			// B10
		DigitalIn sw_right;			// B11
		// Hazard B12
		DigitalIn sw_horn;			// B13

		DigitalOut signal_right;	// D0
		DigitalOut signal_left;		// D1
		DigitalOut signal_head;		// D2
		DigitalOut signal_brake;	// D3
		DigitalOut signal_horn;		// D4 - NO HORN IN Version 1.0

		DigitalOut relay_power;		// D8
		DigitalOut relay_motor;		// D9

#elif NU_PEDALS_VERSION == 15
		/* version 1.5 */
		AnalogIn regen_pedal;		// B0
		AnalogIn accel_pedal;		// B1
		DigitalIn brake_pedal;		// B2
		DigitalIn extra_digi_pedal;	// B3

		AnalogIn sw_extra_analog;	// B4
		DigitalIn sw_extra_digital;	// B5 - using_hardware_switches
		DigitalIn sw_drive;			// B9 - HARDWARE DESIGNED BACKWARDS
		DigitalIn sw_reverse;		// B8
		DigitalIn sw_right;			// B10
		DigitalIn sw_left;			// B11
		DigitalIn sw_head;			// B12
		DigitalIn sw_horn;			// B13

		DigitalOut signal_brake;	// D0
		DigitalOut signal_head;		// D1
		DigitalOut signal_right;	// D2
		DigitalOut signal_left;		// D3
		DigitalOut signal_horn;		// D4

		DigitalOut relay_power;		// D5
		DigitalOut relay_motor;		// D6
		DigitalOut relay_extra;		// D7
#endif

		Nokia5110 lcd;

		Timer send_can_timer;
		Timer bms_can_timer;
		Timer os_can_timer;
		Timer debug_timer;
		Timer precharge_timer;

		struct state_t
		{
			// pedals & derived data
			uint16_t accel_raw;
			uint16_t accel_safe;
			bool brake_en;
			float accel_motor;

			// ignition & gears
			bool key_run;
			bool drive_en, reverse_en, regen_en;

			// signals
			bool lt_left, lt_right, lt_heads, horn;

			// car state
			Precharge bms_state;

			// optional systems
			bool using_hardware_switches;
			float vehicle_velocity;
			float bus_voltage, bus_current;

		} state;

#if 0	//INTERFACE
		Pedals();
		void setup();

		void recv_can();
		void read_ins();
		void set_signals();
		void override_bms();
		void send_can();
		void send_debug();

		void run_loop();
		void emergency_shutoff();
		static void main(Pedals *arena);
#endif

		Pedals():
			nu32		(Nu32::V2011),
			common_can	(CAN1),
			can_in		(common_can, CAN_CHANNEL0),
			can_out_ws20(common_can, CAN_CHANNEL1),
			can_out_bms	(common_can, CAN_CHANNEL2),
			can_out_dc	(common_can, CAN_CHANNEL3),
			regen_pedal	(PIN(B, 0)),
			accel_pedal	(PIN(B, 1)),
			brake_pedal	(PIN(B, 2)),
			sw_drive	(PIN(B, 9)),
			sw_reverse	(PIN(B, 8)),
			sw_right	(PIN(B, 10)),
			sw_left		(PIN(B, 11)),
			sw_head		(PIN(B, 12)),
			sw_horn		(PIN(B, 13)),
			signal_brake	(PIN(D, 0)),
			signal_head		(PIN(D, 1)),
			signal_right	(PIN(D, 2)),
			signal_left		(PIN(D, 3)),
			signal_horn		(PIN(D, 4)),
			relay_power		(PIN(D, 5)),
			relay_motor		(PIN(D, 6)),
			relay_extra		(PIN(D, 7)),
			lcd				(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9), PIN(E, 9)),
			send_can_timer	(NU_PEDALS_CAN_TX_TIMER_MS, Timer::ms, false),
			bms_can_timer	(NU_PEDALS_BMS_TIMEOUT_MS, Timer::ms, false),
			os_can_timer	(NU_PEDALS_OS_TIMEOUT_MS, Timer::ms, false),
			debug_timer		(1, Timer::s, false),
			precharge_timer	(2, Timer::s, false),
			state		()
		{
		}

		void setup()
		{
			WDT::clear();
			nu32.setup();
			common_can.setup();
			common_can.setup_mask(CAN_FILTER_MASK0, 0x7ff);
			common_can.setup_filter(CAN_FILTER0, Can::Addr::os::user_cmds::_id);
			common_can.setup_filter(CAN_FILTER1, Can::Addr::bms1::precharge::_id);
			common_can.setup_filter(CAN_FILTER2, Can::Addr::ws20::motor_velocity::_id);
			common_can.setup_filter(CAN_FILTER3, Can::Addr::ws20::motor_bus::_id);
			can_in.setup_rx();
			can_in.link_filter(CAN_FILTER0, CAN_FILTER_MASK0);
			can_in.link_filter(CAN_FILTER1, CAN_FILTER_MASK0);
			can_in.link_filter(CAN_FILTER2, CAN_FILTER_MASK0);
			can_in.link_filter(CAN_FILTER3, CAN_FILTER_MASK0);
			can_out_ws20.setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			can_out_bms.setup_tx(CAN_LOW_MEDIUM_PRIORITY);
			can_out_dc.setup_tx(CAN_LOWEST_PRIORITY);

			regen_pedal.setup();
			accel_pedal.setup();
			brake_pedal.setup();
			signal_horn.setup();
			relay_power.setup();
			relay_motor.setup();
			signal_brake.setup();
			signal_head.setup();
			signal_right.setup();
			signal_left.setup();
			sw_drive.setup();
			sw_reverse.setup();
			sw_right.setup();
			sw_left.setup();
			sw_horn.setup();
			lcd.setup();

			if (NU_PEDALS_OVERRIDE_BMS) this->state.bms_state = Precharge__Idle;
			// Immediately enable batteries!
			// They are only turned off by an OS packet!
			this->state.key_run = true;
		}

		void recv_can()
		{
			uint32_t id;
			Can::Packet incoming;

			this->can_in.rx(incoming, id);
			switch (id) {
				case Can::Addr::bms1::precharge::_id: {
					Can::Addr::bms1::precharge pkt(incoming);

					// copy state of BMS
					this->state.bms_state = (Precharge) pkt.frame().precharge_state;

					this->bms_can_timer.reset();
					break;
				}
				case Can::Addr::os::user_cmds::_id: {
					Can::Addr::os::user_cmds pkt(incoming);

					// KEEP IN SYNC with flags in /driver-server/SolarCar/DataAggregator.cs
					this->state.key_run		= pkt.frame().gearFlags & 1<<0;
					this->state.drive_en	= pkt.frame().gearFlags & 1<<1;
					this->state.reverse_en	= pkt.frame().gearFlags & 1<<2;
					this->state.regen_en	= !(this->state.drive_en || this->state.reverse_en); // regen when neither
					this->state.lt_left		= pkt.frame().signalFlags & 1<<0;
					this->state.lt_right	= pkt.frame().signalFlags & 1<<1;
					this->state.lt_heads	= pkt.frame().signalFlags & 1<<2;
					this->state.horn		= pkt.frame().signalFlags & 1<<3;
					this->state.using_hardware_switches = false;
					this->os_can_timer.reset();
					break;
				}
				case Can::Addr::ws20::motor_velocity::_id: {
					Can::Addr::ws20::motor_velocity pkt(incoming);
					this->state.vehicle_velocity = pkt.frame().vehicleVelocity;
					break;
				}
				case Can::Addr::ws20::motor_bus::_id: {
					Can::Addr::ws20::motor_bus pkt(incoming);
					this->state.bus_voltage = pkt.frame().busVoltage;
					this->state.bus_current = pkt.frame().busCurrent;
					break;
				}
			}

			// Kill things if BMS times-out.
			if (this->bms_can_timer.has_expired() && !NU_PEDALS_OVERRIDE_BMS)
			{
				this->state.bms_state = Precharge__Error;
			}

			// Kill things if OS times-out.
			if (this->os_can_timer.has_expired())
			{
				this->state.using_hardware_switches = true;
				//Key stays in Run unless OS's CAN packet explicitly changes it!
				//this->state.key_run		= true;
				this->state.drive_en	= false;
				this->state.reverse_en	= false;
				this->state.regen_en	= false;
				this->state.lt_left		= false;
				this->state.lt_right	= false;
				this->state.lt_heads	= false;
				this->state.horn		= false;
			}
		}

		/// Reads the pedal pins, check
		void read_ins()
		{
			WDT::clear();
			// read raw inputs
			this->state.accel_raw = (uint16_t)(this->accel_pedal.read() & 0x3ff); // only 10 meaningful bits returned
			this->state.brake_en = (bool) this->brake_pedal.read();

			// keep accel_raw from overcurrenting
			// if Bus Current is safe, or else if accel_raw is lower, then update accel_raw
			// if Bus Current isn't safe, NEVER increase accel_safe
			if ((this->state.bus_current < NU_PEDALS_CURR_LIMIT-5 &&
				 this->state.bus_current > -NU_PEDALS_CURR_LIMIT+5)
				|| this->state.accel_raw < this->state.accel_safe)
			{
				this->state.accel_safe = this->state.accel_raw;
			}
			
			// process analog pedals
			this->state.accel_motor = clamp(((float)this->state.accel_safe)/1024*NU_PEDALS_ACCEL_LIMIT, 0.0f, 1.0f);
			
			// Use the hardware switches if OS is timed out
			if (this->state.using_hardware_switches)
			{
				this->state.drive_en	= this->sw_drive.read();
				this->state.reverse_en	= this->sw_reverse.read();
				this->state.regen_en	= !(this->state.drive_en || this->state.reverse_en); // regen when neither
				this->state.lt_left		= this->sw_left.read();
				this->state.lt_right	= this->sw_right.read();
				this->state.horn		= this->sw_horn.read();
			}
		}

		void set_signals()
		{
			WDT::clear();
			this->signal_brake.set(this->state.brake_en || this->state.regen_en || this->state.reverse_en);
			this->signal_horn.set(this->state.horn);
			this->signal_head.set(this->state.lt_heads);

			// Toggle turn-signal once per second
			bool tick = timer::s()%2;
			this->signal_left.set(this->state.lt_left? tick: 0);
			this->signal_right.set(this->state.lt_right? tick: 0);
		}

		/** Manual override of BMS. FOR LEAD ACID BATTERIES ONLY!!! */
		void override_bms()
		{
			// First, check Trip points if Running
			if (this->state.bms_state == Precharge__Run &&
					((this->state.bus_voltage > 60.0f &&
					this->state.bus_voltage < 110.0f) ||
					this->state.bus_current > NU_PEDALS_CURR_LIMIT ||
					this->state.bus_current < -NU_PEDALS_CURR_LIMIT)
				)
			{
				// If tripped, permanently set Error
				this->state.bms_state = Precharge__Error;
				this->precharge_timer.kill();
				this->relay_motor.low();
				this->relay_power.low();
				// Send a CAN packet ASAP with Trip values
				this->send_can_timer.kill();
			}
			else if (this->state.bms_state == Precharge__Error &&
					(
						(this->state.bus_voltage < 60.0f ||
						this->state.bus_voltage > 110.0f) &&
						this->state.bus_current < NU_PEDALS_CURR_LIMIT &&
						this->state.bus_current > -NU_PEDALS_CURR_LIMIT
					)
				)
			{
				this->state.bms_state = Precharge__Idle;
			}

			// Then, if health is OK, proceed
			if (this->state.bms_state != Precharge__Error)
			{
				// Enable relays if carkey is in Run
				if (this->state.key_run)
				{
					if (this->state.bms_state == Precharge__Idle)
					{
						this->relay_motor.low();
						this->relay_power.high();
						this->precharge_timer.reset();
						this->state.bms_state = Precharge__Precharge;
					}
					else if (this->state.bms_state == Precharge__Precharge)
					{
						this->relay_motor.low();
						this->relay_power.high();
						if (this->precharge_timer.has_expired())
						{
							this->state.bms_state = Precharge__Run;
						}
					}
					else if (this->state.bms_state == Precharge__Run)
					{
						this->relay_power.high();
						this->relay_motor.high();
					}
				}
				else
				{
					this->relay_motor.low();
					this->relay_power.low();
					this->precharge_timer.kill();
					this->state.bms_state = Precharge__Idle;
				}
			}
		}

		void send_can()
		{
			WDT::clear();

			if (this->send_can_timer.has_expired())
			{
				// Initialize Drive Command with zero [velocity, current]
				Can::Addr::dc::drive_cmd drive(0);
				// if BMS is precharged & User is in Drive Gear
				if ((this->state.bms_state == Precharge__Run)
					&& !this->state.brake_en
					&& this->state.accel_motor > NU_PEDALS_ACCEL_SENSITIVITY)
				{
					
					if (this->state.reverse_en)
					{
						drive.frame().motorVelocity = -100.0f; // Max Reverse 100m/s
					}
					else if (this->state.drive_en)
					{
						drive.frame().motorVelocity = 100.0f; // Max 100m/s
					}
					else if (this->state.regen_en)
					{
						drive.frame().motorVelocity = 0.0f;
					}
					drive.frame().motorCurrent = this->state.accel_motor; // accel percent
				}
				this->can_out_ws20.tx(drive);

				// Also send a BMS packet
				Can::Addr::dc::switches switches(0);
				switches.frame().switchFlags = (uint16_t)(1<<4); // Accessories
				// if User has Powered battery pack, start BMS, for state.bms_run
				if (this->state.key_run == true)
					switches.frame().switchFlags |= (uint16_t)(1<<5); // Run
				if (this->state.key_run && (this->state.bms_state != Precharge__Run))
					switches.frame().switchFlags |= (uint16_t)(1<<6); // Start
				this->can_out_bms.tx(switches);

				// Also send a pedals update
				Can::Addr::dc::pedals report(0);
				report.frame().accel_pedal = this->state.accel_raw;
				report.frame().brake_pedal = this->state.brake_en;
				this->can_out_dc.tx(report);

				this->send_can_timer.reset();
			}
		}

		void send_debug()
		{
			WDT::clear();
			if (this->debug_timer.has_expired())
			{
				this->nu32.led1.toggle();
				
				/*this->lcd.lcd_clear();
				this->lcd.printf("ZELDA");
				this->lcd.goto_xy(0, 1);
				this->lcd.printf("Ac-B-SKDR");
				this->lcd.goto_xy(0, 2);
				this->lcd.printf("%hu-%hu-%hu%hu%hu%hu",
					this->state.accel_raw, this->state.brake_en,
					this->state.bms_state, this->state.key_run, this->state.drive_en, this->state.reverse_en);
				this->lcd.goto_xy(0, 3);
				this->lcd.printf("vel=%2.1f", (double)this->state.vehicle_velocity);
				this->lcd.goto_xy(0, 4);
				this->lcd.printf("LRHHd-%hu%hu%hu%hu",
					this->state.lt_left, this->state.lt_right, this->state.horn, this->state.lt_heads);*/
				
				this->nu32.serial_usb1.printf("ZELDA\tAc-Br\t%4.hu-%hu\tState-Key-D-R-G\t%hu-%hu-%hu-%hu-%hu\tLRHHd = %hu%hu%hu%hu\tvel=%3.1f\tvolt=%3.1f\tcurr=%3.1f\n",
					this->state.accel_raw, this->state.brake_en,
					this->state.bms_state, this->state.key_run, this->state.drive_en, this->state.reverse_en, this->state.regen_en,
					this->state.lt_left, this->state.lt_right, this->state.horn, this->state.lt_heads,
					this->state.vehicle_velocity, this->state.bus_voltage, this->state.bus_current);
				this->debug_timer.reset();
			}
		}

		NORETURN void run_loop()
		{
			while (true)
			{
				WDT::clear();
				this->recv_can();
				this->read_ins();
				this->set_signals();
				if (NU_PEDALS_OVERRIDE_BMS) this->override_bms();
				this->send_can();
				this->send_debug();
				timer::delay_ms(1);
			}
		}

		void emergency_shutoff()
		{
			Can::Addr::dc::drive_cmd drive(0);
			this->can_out_ws20.tx(drive);
			Can::Addr::dc::switches switches(0);
			this->can_out_dc.tx(switches);
		}

		static NORETURN void main(Pedals *arena);
	};
}

#endif	/* NUXX_PEDALS_HPP */


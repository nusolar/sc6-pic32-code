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

namespace nu
{
	struct Pedals
	{
		Nu32 nu32;

		Can::Module common_can;

		AnalogIn regen_pedal, accel_pedal; // B0, B1
		DigitalIn brake_pedal; // B2

		DigitalOut horn; // B13
		DigitalOut lights_r; //	D0
		DigitalOut lights_l; // D1
		DigitalOut lights_head; // D2
		DigitalOut lights_brake; // D3

		DigitalIn sw_drive; // B8
		DigitalIn sw_reverse; // B9
		DigitalIn sw_left; // B10
		DigitalIn sw_right; // B11
		DigitalIn sw_horn; // B13

		Nokia5110 lcd;

		Timer ws20_timer;
		Timer os_timer;
		Timer lcd_timer;

		struct state_t
		{
			// pedals & derived data
			uint16_t accel_raw;
			bool brake_en;
			float accel_motor;

			// ignition & gears
			bool battery_en, drive_en, reverse_en, regen_en;
			bool bms_ready;

			// signals
			bool lt_left, lt_right, lt_heads, horn;

			// other data
			bool hardware_switch;
			float vehicle_velocity;
		} state;

#if 0 //INTERFACE
		Pedals();
		void setup();
		void recv_can();
		void read_ins();
		void set_signals();
		void send_can();
		void draw_lcd();
		void run_loop();
		void emergency_shutoff();
		static void main(Pedals *arena);
#endif

		Pedals():
			nu32		(Nu32::V2011),
			common_can	(CAN1),
			regen_pedal	(PIN(B, 0)),
			accel_pedal	(PIN(B, 1)),
			brake_pedal	(PIN(B, 2)),
			horn		(PIN(B, 13)),
			lights_r	(PIN(D, 0)),
			lights_l	(PIN(D, 1)),
			lights_head	(PIN(D, 2)),
			lights_brake(PIN(D, 3)),
			sw_drive	(PIN(B, 8)),
			sw_reverse	(PIN(B, 9)),
			sw_left		(PIN(B, 10)),
			sw_right	(PIN(B, 11)),
			sw_horn		(PIN(B, 13)),
			lcd			(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9), PIN(E, 9)),
			ws20_timer	(NU_PEDALS_WS20_TIMEOUT_MS, Timer::ms, false),
			os_timer	(NU_PEDALS_OS_TIMEOUT_MS, Timer::ms, false),
			lcd_timer	(500, Timer::ms, false),
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
			common_can.in().setup_rx();
			common_can.in().link_filter(CAN_FILTER0, CAN_FILTER_MASK0);
			common_can.in().link_filter(CAN_FILTER1, CAN_FILTER_MASK0);
			common_can.in().link_filter(CAN_FILTER2, CAN_FILTER_MASK0);
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.channel(2).setup_tx(CAN_LOW_MEDIUM_PRIORITY);
			common_can.channel(3).setup_tx(CAN_LOWEST_PRIORITY);
			regen_pedal.setup();
			accel_pedal.setup();
			brake_pedal.setup();
			horn.setup();
			lights_r.setup();
			lights_l.setup();
			lights_head.setup();
			lights_brake.setup();
			sw_drive.setup();
			sw_reverse.setup();
			sw_left.setup();
			sw_right.setup();
			sw_horn.setup();
			lcd.setup();
		}

		void recv_can()
		{
			uint32_t id;
			Can::Packet incoming;

			common_can.in().rx(incoming, id);
			switch (id) {
				case Can::Addr::bms1::precharge::_id: {
					Can::Addr::bms1::precharge pkt(incoming);
					this->state.bms_ready = (pkt.frame().precharge_state == 4);
					break;
				}
				case Can::Addr::os::user_cmds::_id: {
					// KEEP IN SYNC with flags in /driver-server/SolarCar/DataAggregator.cs
					Can::Addr::os::user_cmds pkt(incoming);
					this->state.battery_en	= pkt.frame().gearFlags | 1<<0;
					this->state.drive_en	= pkt.frame().gearFlags | 1<<1;
					this->state.reverse_en	= pkt.frame().gearFlags | 1<<2;

					this->state.lt_left		= pkt.frame().signalFlags | 1<<0;
					this->state.lt_right	= pkt.frame().signalFlags | 1<<1;
					this->state.lt_heads	= pkt.frame().signalFlags | 1<<2;
					this->state.horn		= pkt.frame().signalFlags | 1<<3;

					this->os_timer.reset();
					break;
				}
				case Can::Addr::ws20::motor_velocity::_id: {
					Can::Addr::ws20::motor_velocity pkt(incoming);
					this->state.vehicle_velocity = pkt.frame().vehicleVelocity;
					break;
				}
			}

			// Kill things if OS times-out.
			if (this->os_timer.has_expired() && !this->state.hardware_switch)
			{
				this->state.battery_en	= false;
				this->state.drive_en	= false;
				this->state.reverse_en	= false;
				this->state.lt_left		= false;
				this->state.lt_right	= false;
				this->state.lt_heads	= false;
				this->state.horn		= false;
			}
		}

		void read_ins()
		{
			WDT::clear();
			// read raw inputs
			this->state.accel_raw = (uint16_t)(this->accel_pedal.read() & 0x3ff); // only 10 meaningful bits returned
			this->state.brake_en = (bool) this->brake_pedal.read();
			// process analog pedals
			this->state.accel_motor = ((float)this->state.accel_raw)/1024;

			// Use the hardware switches if they're connected
			this->state.hardware_switch = (bool) this->sw_drive.read();
			if (this->state.hardware_switch)
			{
				this->state.bms_ready	= true; // WARNING DEBUG
				this->state.battery_en	= true; // no switch --> always assumed
				this->state.drive_en	= this->state.hardware_switch;
				this->state.reverse_en	= this->sw_reverse.read();
				this->state.lt_left		= this->sw_left.read();
				this->state.lt_right	= this->sw_right.read();
				this->state.horn		= this->sw_horn.read();
			}
		}

		void set_signals()
		{
			WDT::clear();
			this->lights_brake.set(state.brake_en || state.reverse_en);
			this->horn.set(state.horn);
			this->lights_head.set(state.lt_heads);

			// Toggle turn-signal once per second
			bool tick = timer::s()%2;
			this->lights_l.set(this->state.lt_left? tick: 0);
			this->lights_r.set(this->state.lt_right? tick: 0);
		}

		void send_can()
		{
			WDT::clear();

			if (this->ws20_timer.has_expired())
			{
				// Initialize Drive Command with zero [velocity, current]
				Can::Addr::dc::drive_cmd drive(0);
				// if BMS is precharged & User is in Drive Gear
				if (state.drive_en && state.bms_ready)
				{
					if (state.brake_en)
					{
						if (state.regen_en)
						{
							drive.frame().motorVelocity = 0;
							drive.frame().motorCurrent = NU_PEDALS_REGEN_PROPORTION;
						}
					}
					else if (state.accel_motor > NU_PEDALS_ACCEL_SENSITIVITY)
					{
						drive.frame().motorVelocity = 100; // Max 100m/s
						drive.frame().motorCurrent = state.accel_motor; // accel percent
					}

					if (state.reverse_en)
					{
						drive.frame().motorVelocity *= -1;
					}
				}
				this->common_can.out().tx(drive);

				// Also send a BMS packet
				Can::Addr::dc::switches switches(0);
				switches.frame().switchFlags = (uint16_t)(1<<4); // Accessories
				// if User has Powered battery pack, start BMS, for state.bms_run
				if (state.battery_en == true)
					switches.frame().switchFlags |= (uint16_t)(1<<5); // Run
				if (state.battery_en && !state.bms_ready)
					switches.frame().switchFlags |= (uint16_t)(1<<6); // Start
				this->common_can.channel(2).tx(switches);

				// Also send a pedals update
				Can::Addr::dc::pedals report(0);
				report.frame().accel_pedal = this->state.accel_raw;
				report.frame().brake_pedal = this->state.brake_en;
				this->common_can.channel(3).tx(report);

				this->ws20_timer.reset();
			}
		}

		void draw_lcd()
		{
			WDT::clear();
			if (this->lcd_timer.has_expired())
			{
				this->nu32.led1.toggle();
				this->lcd.lcd_clear();
				this->lcd.printf("ZELDA");
				this->lcd.goto_xy(0, 1);
				this->lcd.printf("Ac-B-PcDR");
				this->lcd.goto_xy(0, 2);
				this->lcd.printf("%hu-%hu-%hu%hu%hu",
					this->state.accel_raw, this->state.brake_en, this->state.bms_ready, this->state.drive_en, this->state.reverse_en);
				this->lcd.goto_xy(0, 3);
				this->lcd.printf("vel=%2.1f", (double)this->state.vehicle_velocity);
				this->lcd.goto_xy(0, 4);
				this->lcd.printf("LRHHd-%hu%hu%hu%hu",
					this->state.lt_left, this->state.lt_right, this->state.horn, this->state.lt_heads);
//				this->nu32.serial_usb1.printf("ZELDA\nAc-Br-PcGr\n%hu-%hu-%hu-%hu\nvel=%2.1f\n",
//					this->state.accel_raw, this->state.brake_en, this->state.bms_ready, this->state.drive_en,
//					this->state.vehicle_velocity);
				this->lcd_timer.reset();
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
				this->send_can();
				this->draw_lcd();
				timer::delay_ms(5);
			}
		}

		void emergency_shutoff()
		{
			Can::Addr::dc::drive_cmd drive(0);
			this->common_can.out().tx(drive);
			Can::Addr::dc::switches switches(0);
			this->common_can.channel(2).tx(switches);
		}

		static NORETURN void main(Pedals *arena);
	};
}

#endif	/* NUXX_PEDALS_HPP */


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
#include "nuxx/peripheral/pinctl.hpp"
#include "nuxx/peripheral/can.hpp"
#include "nuxx/wdt.hpp"
#include "nuxx/timer.hpp"
#include "nu/config.h"
#include "nu/compiler.h"

namespace nu
{
	struct Pedals: Nu32
	{
		Can::Module common_can;
		AnalogIn regen_pedal, accel_pedal;
		DigitalIn brake_pedal;

		DigitalOut horn; // B13
		DigitalOut lights_r; //	D0
		DigitalOut lights_l; // D1
		DigitalOut headlights; // D2
		DigitalOut lights_brake; // D3

		Nokia5110 lcd;

		Timer ws20_timer;
		Timer os_timer;
		Timer lcd_timer;

		struct state_t
		{
			uint16_t accel_raw;
			bool brake_en;

			float accel_motor;
			float cruise;

			bool bps_drive;
			bool gear, reverse_en, regen_en, cruise_en;
			bool left, right, headlights, horn;
		} state;

		Pedals():
			Nu32(Nu32::V2011),
			common_can(CAN1),
			regen_pedal(PIN(B, 0)),
			accel_pedal(PIN(B, 1)),
			brake_pedal(PIN(B, 2)),
			horn(PIN(B, 13)),
			lights_r(PIN(D, 0)),
			lights_l(PIN(D, 1)),
			headlights(PIN(D, 2)),
			lights_brake(PIN(D, 3)),
			lcd(PIN(G, 9), SPI_CHANNEL2, PIN(A, 9), PIN(E, 9)),
			ws20_timer(NU_PEDALS_WS20_TIMEOUT_MS, Timer::ms, false),
			os_timer(NU_PEDALS_OS_TIMEOUT_MS, Timer::ms, false),
			lcd_timer(1, Timer::s, false),
			state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.in().add_filter(CAN_FILTER0, CAN_SID, Can::Addr::os::user_cmds::_id,
				CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7FF);
			common_can.in().add_filter(CAN_FILTER1, CAN_SID, Can::Addr::bps_tx::bps_status::_id,
				CAN_FILTER_MASK1, CAN_FILTER_MASK_IDE_TYPE, 0x7FF);
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}

		void recv_can()
		{
			uint32_t id;
			Can::Packet incoming;

			common_can.in().rx(incoming, id);
			switch (id) {
				case Can::Addr::bps_tx::bps_status::_id: {
					Can::Addr::bps_tx::bps_status pkt(incoming);
					this->state.bps_drive = pkt.frame().mode | 1<<1;
					break;
				}
				case Can::Addr::os::user_cmds::_id: {
					// KEEP IN SYNC with flags in /driver-server/SolarCar/DataAggregator.cs
					Can::Addr::os::user_cmds pkt(incoming);
					this->state.gear = pkt.frame().gearFlags | 1<<0;
					this->state.reverse_en = pkt.frame().gearFlags | 1<<1;

					this->state.left = pkt.frame().signalFlags | 1<<0;
					this->state.right = pkt.frame().signalFlags | 1<<1;
					this->state.headlights = pkt.frame().signalFlags | 1<<2;
					this->state.horn = pkt.frame().signalFlags | 1<<3;

					os_timer.reset();
					break;
				}
				default: {
					// Kill things if OS times-out.
					if (os_timer.has_expired())
					{
						this->state.gear = false;
						this->state.reverse_en = false;

						this->state.left = false;
						this->state.right = false;
						this->state.headlights = false;
						this->state.horn = false;
					}
					break;
				}
			}
		}

		void read_ins()
		{
			WDT::clear();
			// read raw inputs
			state.accel_raw = (uint16_t) accel_pedal.read();
			state.brake_en = (bool) brake_pedal.read();
			// process analogs
			state.accel_motor = ((float)state.accel_raw + 1)/1024;
		}

		void set_signals()
		{
			WDT::clear();
			this->lights_brake.set(state.brake_en);
			this->horn.set(state.horn);
			this->headlights.set(state.headlights);

			// Toggle turn-signal once per second
			bool tick = timer::s()%2;
			this->lights_l.set(this->state.left? tick: 0);
			this->lights_r.set(this->state.right? tick: 0);
		}

		void send_can()
		{
			WDT::clear();

			if (this->ws20_timer.has_expired())
			{
				// Initialize Drive Command with zero [current, velocity]
				Can::Addr::dc::drive_cmd drive(0);
				// need clearance from BPS & OS to drive
				if (state.bps_drive && state.gear)
				{
					if (state.brake_en)
					{
						state.cruise_en = 0;
						if (state.regen_en)
						{
							drive.frame().motorCurrent = NU_PEDALS_REGEN_PROPORTION;
						}
						else
						{
							Nop(); // Normal braking.
						}
					}
					else if (state.cruise_en)
					{
						//drive.frame.contents = {state.cruise, 1};
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
				switches.frame().switchFlags = (uint16_t)(1<<4);
				if (state.gear == true)
					switches.frame().switchFlags |= (uint16_t)(1<<5); // Run
				if (false)
					switches.frame().switchFlags |= (uint16_t)(1<<6); // Start

				// Also send a pedals update
				Can::Addr::dc::pedals report(0);
				report.frame().accel_pedal = (this->state.accel_raw) | 0x3ff;
				report.frame().brake_pedal = this->state.brake_en;
				this->common_can.out().tx(report);
			}
		}

		void draw_lcd()
		{
			if (this->lcd_timer.has_expired())
			{
				this->led1.toggle();
				this->lcd.lcd_clear();
				this->lcd.printf("ZELDA");
				this->lcd.goto_xy(0, 1);
				this->lcd.printf("%hu-%hhu", this->state.accel_raw, this->state.brake_en);
				this->serial1.printf("ZELDA\n%hu-%hhu\n", this->state.accel_raw, this->state.brake_en);
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
			}
		}

		void emergency_shutoff()
		{
			Can::Addr::dc::drive_cmd drive(0);
			this->common_can.out().tx(drive);
		}

		static NORETURN void main(Pedals *arena);
	};
}

#endif	/* NUXX_PEDALS_HPP */


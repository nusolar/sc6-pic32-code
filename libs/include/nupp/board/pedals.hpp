/*
 * File:   pedals.hpp
 * Author: alex
 *
 * Created on September 29, 2013, 9:22 PM
 */

#ifndef NUPP_PEDALS_HPP
#define	NUPP_PEDALS_HPP

#include "nupp/peripheral/pinctl.hpp"
#include "nupp/peripheral/can.hpp"
#include "nupp/wdt.hpp"
#include "nupp/timer.hpp"
#include "nu/config.h"
#include "nu/compiler.h"

namespace nu
{
	struct Pedals
	{
		can::Module common_can;
		AnalogIn regen_pedal, accel_pedal;
		DigitalIn brake_pedal;

		DigitalOut horn; // B13
		DigitalOut lights_r; //	D0
		DigitalOut lights_l; // D1
		DigitalOut headlights; // D2
		DigitalOut lights_brake; // D3

		Timer ws20_timer;
		Timer os_timer;

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
			common_can(CAN1),
			regen_pedal(PIN(B, 0)),
			accel_pedal(PIN(B, 1)),
			brake_pedal(PIN(B, 2)),
			horn(PIN(B, 13)),
			lights_r(PIN(D, 0)),
			lights_l(PIN(D, 1)),
			headlights(PIN(D, 2)),
			lights_brake(PIN(D, 3)),
			ws20_timer(NU_PEDALS_WS20_TIMEOUT_MS, Timer::ms, false),
			os_timer(NU_PEDALS_OS_TIMEOUT_MS, Timer::ms, false),
			state()
		{
			WDT::clear();
#warning "Need to receive BPS packets too!"
			common_can.in().setup_rx();
			common_can.in().add_filter(CAN_FILTER0, CAN_SID, can::frame::os::tx::driver_input_k,
				CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7FF);
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}

		INLINE void recv_can()
		{
			uint32_t id;
			can::frame::Packet incoming;

			common_can.in().rx(incoming, id);
			switch (id) {
				case (uint32_t)can::frame::bps::tx::bps_status_k: {
					can::frame::bps::tx::bps_status pkt(incoming);
					this->state.bps_drive = pkt.frame().mode | 1<<1;
					break;
				}
				case (uint32_t)can::frame::os::tx::driver_input_k: {
					// KEEP IN SYNC with flags in /driver-server/SolarCar/DataAggregator.cs
					can::frame::os::tx::driver_input pkt(incoming);
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

		INLINE void read_ins()
		{
			WDT::clear();
			// read raw inputs
			state.accel_raw = (uint16_t) accel_pedal.read();
			state.brake_en = (bool) brake_pedal.read();
			// process analogs
			state.accel_motor = ((float)state.accel_raw + 1)/1024;
		}

		INLINE void set_signals()
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

		INLINE void send_can()
		{
			WDT::clear();

			if (ws20_timer.has_expired())
			{
				// Initialize Drive Command with zero [current, velocity]
				can::frame::ws20::rx::drive_cmd drive(0);
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
				common_can.out().tx(drive);

				// Also send a pedals update
				can::frame::pedals::tx::pedals report(0);
				report.frame().accel_pedal = (this->state.accel_raw) | 0x3ff;
				report.frame().brake_pedal = this->state.brake_en;
				common_can.out().tx(report);
			}
		}

		INLINE NORETURN void run_loop()
		{
			while (true)
			{
				WDT::clear();
				this->recv_can();
				this->read_ins();
				this->set_signals();
				this->send_can();
			}
		}

		INLINE void emergency_shutoff()
		{
			can::frame::ws20::rx::drive_cmd drive(0);
			common_can.out().tx(drive);
		}

		static NORETURN void main(Pedals *arena);
	};
}

#endif	/* NUPP_PEDALS_HPP */


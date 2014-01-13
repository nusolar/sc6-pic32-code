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
#include "nu/compiler.h"

namespace nu {
	struct Pedals {
		static const uint64_t OS_TIMEOUT_MS = 100; // 100ms

		can::Module common_can;
		AnalogIn regen_pedal, accel_pedal;
		DigitalIn brake_pedal;

		DigitalOut horn; // B13
		DigitalOut lights_r; //	D0
		DigitalOut lights_l; // D1
		DigitalOut headlights; // D2
		DigitalOut lights_brake; // D3

		struct state_t {
			uint16_t accel_raw;
			bool brake_en;

			float accel_motor;
			float cruise;

			bool gear, reverse_en, regen_en, cruise_en;
			bool left, right, headlights, horn;

			uint64_t os_timer;
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
			state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.in().add_filter(CAN_FILTER0, CAN_SID, 0x310, CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7FF); // 0x310 == OS status
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}

		ALWAYSINLINE void recv_can() {
			uint32_t id;
			can::frame::Packet incoming;

			common_can.in().rx(incoming, id);
			switch (id) {
				case (uint32_t)can::frame::os::tx::driver_input_k: {
					can::frame::os::tx::driver_input pkt(incoming);
					this->state.gear = pkt.frame().gearFlags | 1<<0;
					this->state.reverse_en = pkt.frame().gearFlags | 1<<1;
					this->state.horn = pkt.frame().horn;
					this->state.left = pkt.frame().lightsFlags | 1<<0;
					this->state.right = pkt.frame().lightsFlags | 1<<1;
					this->state.headlights = pkt.frame().lightsFlags | 1<<2;
					
				}

				default: {
					// Kill things if OS times-out.
					uint32_t os_time = (uint32_t)timer::ms();
					if ((os_time > this->state.os_timer + OS_TIMEOUT_MS)||
							(os_time < this->state.os_timer && os_time > OS_TIMEOUT_MS)) {
						this->state.gear = false;
						this->state.reverse_en = false;
						this->state.horn = false;
						this->state.headlights = false;
						this->state.left = false;
						this->state.right = false;
					}
				}
			}
		}

		ALWAYSINLINE void read_ins() {
			WDT::clear();
			// read raw inputs
			state.accel_raw = (uint16_t) accel_pedal.read();
			state.brake_en = (bool) brake_pedal.read();
			// process analogs
			state.accel_motor = ((float)state.accel_raw + 1)/1024;
		}

		ALWAYSINLINE void set_signals() {
			WDT::clear();
			this->lights_brake.set(state.brake_en);
			this->horn.set(state.horn);
			this->headlights.set(state.headlights);
			
			// Toggle turn-signal once per second
			bool tick = timer::s()%2;
			this->lights_l.set(this->state.left? tick: 0);
			this->lights_r.set(this->state.right? tick: 0);
		}

		ALWAYSINLINE void send_can() {
			WDT::clear();

			can::frame::ws20::rx::drive_cmd drive(0); // Zero-init [current, velocity]

			if (state.gear) {
				if (state.brake_en) {
					state.cruise_en = 0;
					if (state.regen_en){
						drive.frame().motorCurrent = 0.2; // REGEN_AMOUNT
					} else {
						Nop(); // Normal braking.
					}
				} else if (state.cruise_en) {
					//drive.frame.contents = {state.cruise, 1};
				} else if (state.accel_motor > 0.05) {
					drive.frame().motorVelocity = 101; // Max 101m/s
					drive.frame().motorCurrent = state.accel_motor; // accel percent
				}

				if (state.reverse_en) {
					drive.frame().motorVelocity *= -1;
				}
			}

			common_can.out().tx(drive);

			can::frame::pedals::tx::pedals report(0);
			report.frame().accel_pedal = (this->state.accel_raw) | 0x3ff;
			report.frame().brake_pedal = this->state.brake_en;
			common_can.out().tx(report);
		}

		ALWAYSINLINE NORETURN void run_loop() {
			while (true) {
				WDT::clear();
				this->recv_can();
				this->read_ins();
				this->set_signals();
				this->send_can();
			}
		}

		INLINE void emergency_shutoff() {
			can::frame::ws20::rx::drive_cmd drive(0);
			common_can.out().tx(drive);
		}

		static NORETURN void main(Pedals *arena);
	};
}

#endif	/* NUPP_PEDALS_HPP */


/* 
 * File:   pedals.hpp
 * Author: alex
 *
 * Created on September 29, 2013, 9:22 PM
 */

#ifndef NUPP_PEDALS_HPP
#define	NUPP_PEDALS_HPP

#include "nupp/pinctl.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"
#include "nu/compiler.h"

namespace nu {
	struct Pedals {
		can::Module common_can;
		AnalogIn regen_pedal, accel_pedal;
		DigitalIn brake_pedal;

		struct state_t {
			uint16_t accel;
			bool brake;
		} state;

		Pedals():
			common_can(CAN1),
			regen_pedal(PIN(B, 0)),
			accel_pedal(PIN(B, 1)),
			brake_pedal(PIN(B, 2)),
			state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.in().add_filter(CAN_FILTER0, CAN_SID, 0x312, CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7FF); // 0x312 == SW btns
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}

		ALWAYSINLINE void read_ins() {
			WDT::clear();
			state.accel = (uint16_t) accel_pedal.read();
			state.brake = (bool) brake_pedal.read();
		}

		ALWAYSINLINE void send_can() {
			WDT::clear();
		}

		ALWAYSINLINE NORETURN void run_loop() {
			while (true) {
				WDT::clear();
				this->read_ins();
				this->send_can();
			}
		}
	};
}

#endif	/* NUPP_PEDALS_HPP */


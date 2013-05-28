//
//  driver_controls.h
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__driver_controls__
#define __nusolar_lib__driver_controls__

#include "nu/compiler.h"
#include "nupp/nokia5110.hpp"// MAXIMUM WARNING MAXIMUM ERROR MUST COME BEFORE NU32
#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"
#include <cstdint>
#include <cstddef>


#define DC_PINS(X)\
	X(AnalogIn, regen_pedel,	B,	0)\
	X(AnalogIn, accel_pedel,	B,	1)\
	X(AnalogIn, airgap_pot,		B,	4)\
	X(DigitalIn, brake_pedal,		B,	2)\
	X(DigitalIn, headlight_switch,	B,	3)\
	X(DigitalIn, airgap_switch,		B,	5)\
	X(DigitalIn, regen_switch,		B,	8)\
	X(DigitalIn, reverse_switch,	B,	9)\
	X(DigitalIn, left_sig,			B,	10)\
	X(DigitalIn, right_sig,			B,	11)\
	X(DigitalIn, hazard_sig,		B,	12)\
	X(DigitalOut, lights_brake,	D,	0)\
	X(DigitalOut, lights_l,		D,	1)\
	X(DigitalOut, lights_r,		D,	2)\
	X(DigitalOut, headlights,	D,	3)

#define DC_DECLARE(Type, name, ltr, num) Type name;
#define DC_INITIALIZE(Type, name, ltr, num) name(Pin(Pin::ltr, num)),


namespace nu {
	struct DriverControls: public Nu32 {
		DC_PINS(DC_DECLARE) // FUCK MPLAB
		can::Module ws_can, common_can;
		Nokia5110 lcd;

		/**
		 * State of DriverControls-relevant parameters.
		 * TODO: Implement extensible, polymorphic, car-wide state management.
		 */
		struct state {
			bool lights_l, lights_r, lights_hazard,
			lights_head, lights_brake, horn, // HORN DISCONNECTED
			accel_en, brake_en, reverse_en, regen_en, airgap_en, cruise_en;
			float accel, regen, airgap, cruise; // FUCK MPLAB
			uint32_t sw_timer;

			ALWAYSINLINE state(): lights_l(0), lights_r(0), lights_hazard(0),
				lights_head(0), lights_brake(0), horn(0),
				accel_en(0), brake_en(0), reverse_en(0), regen_en(0), airgap_en(0), cruise_en(0),
				accel(0), regen(0), airgap(0), cruise(0), sw_timer(0) {}
		} state;

		/**
		 * Setup CAN, input Pins, output Pins, and Nokia LCD.
		 */
		ALWAYSINLINE DriverControls(): Nu32(Nu32::V1), DC_PINS(DC_INITIALIZE)
			ws_can(CAN1), common_can(CAN2),
			lcd(Pin(Pin::G, 9), SPI_CHANNEL2, Pin(Pin::A, 9), Pin(Pin::E, 9)), state()
		{
			WDT::clear();
			common_can.in().setup_rx();
			common_can.in().add_filter(CAN_FILTER0, CAN_SID, 0x312, CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7FF);
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY); // error reporting channel
			ws_can.in().setup_rx();
			ws_can.in().add_filter(CAN_FILTER0, CAN_SID, 0x404, CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7FC);
			ws_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
		}


		/**
		 * Read all input Pins. Store result in this->state.
		 */
		void ALWAYSINLINE read_ins() {
			WDT::clear();
			
			state.accel = ((float)accel_pedel.read() + 0)/1024; // scale 0-1023 to 0-1
			if (state.accel < 0) state.accel = 0; // TODO: print warning, clamp
			if (state.accel > 1) state.accel = 1; // TODO: print warning
			state.accel_en = state.accel > 0.05;

			state.reverse_en = (bool)reverse_switch.read();

			state.regen = ((float)regen_pedel.read() + 0)/1024; // WARNING: disconnected
			state.regen_en = (bool)regen_switch.read(); // TODO: clamp

			state.airgap = ((float)airgap_pot.read() + 0)/1024; // WARNING: disconnected
			state.airgap_en = (bool)airgap_switch.read(); // TODO: clamp
			
			state.brake_en		= (bool)brake_pedal.read();
			state.lights_brake = state.brake_en;

			state.lights_head = (bool)headlight_switch.read();

			state.lights_l = (bool)left_sig.read();
			state.lights_r = (bool)right_sig.read();
			state.lights_hazard = (bool)hazard_sig.read();
		}


		/**
		 * Read car state from CAN.
		 */
		void ALWAYSINLINE recv_can() {
			uint32_t id;
			can::frame::Packet incoming;
			common_can.in().rx(incoming.bytes(), id);
			switch (id) {
				case (uint32_t)can::addr::sw::tx::buttons_k: {
					can::frame::sw::tx::buttons btns(incoming);
//					state.lights_l = btns.frame.s.left;
//					state.lights_r = btns.frame.s.right;
//					state.lights_hazard = btns.frame.s.hazard;
					state.sw_timer = timer::ms(); // Reset SW time-out
					return;
				}
				default:
					uint32_t dc_time = timer::ms(); // Kill SW things if SW times-out.
					if ((dc_time > state.sw_timer + 500) || ((dc_time < state.sw_timer) && (dc_time > 500))) {
//						state.lights_l = 0;
//						state.lights_r = 0;
//						state.lights_hazard = 0;
						state.cruise_en = 0;
					}
			}
		}


		/**
		 * Update all light Outputs, to conform to state.
		 */
		void ALWAYSINLINE set_lights() {
			WDT::clear();
			headlights		= state.lights_head;
			lights_brake	= state.lights_brake;

			bool tick = timer::s()%2;// Even or Odd, change every second
			lights_l = state.lights_l||state.lights_hazard? tick: 0;
			lights_r = state.lights_r||state.lights_hazard? tick: 0;
		}


		/**
		 * Command motor, from accel_pedel, brake, & cruise control input.
		 */
		void ALWAYSINLINE set_motor() {
			WDT::clear();

			can::frame::ws20::rx::drive_cmd drive{}; // Zero-init [current, velocity]

			if (state.brake_en) {
				state.cruise_en = 0;
				if (state.regen_en)
					drive.frame.s.motorCurrent = 0.2; // REGEN_AMOUNT
				else
					Nop(); // Normal braking.
			} else if (state.accel_en)
				drive.frame.s = {101, state.accel}; // [Max 101m/s, accel percent]

			if (state.reverse_en)
				drive.frame.s.motorVelocity *= -1;

			led1.on(); timer::delay_ms<1>(); led1.off(); // WARNING: bottleneck
			ws_can.out().tx(drive.bytes(),
							8,
							(uint16_t)can::addr::ws20::rx::drive_cmd_k);
		}


		/**
		 * A function to be called repeatedly.
		 */
		void ALWAYSINLINE run() {
			WDT::clear();

			read_ins();
			recv_can();
			set_lights();
			set_motor();

			lcd.lcd_clear();
			lcd.goto_xy(0, 0);
			lcd << 25.0 << end;
		}


		/** Demo LED toggling */
		void ALWAYSINLINE demo() {
			WDT::clear();
			lcd.lcd_clear();
			lcd << (uint32_t)69 << "C++WINS " << timer::s() <<  end;
			
			can::frame::bms::tx::trip tap;
			tap.frame.s.trip_code = 3;
			tap.frame.s.module = timer::s();
			ws_can.out().tx(tap.bytes(), 8, can::addr::bms::tx::trip_k);

			static can::frame::Packet frame;
			static uint32_t id = 0;
			ws_can.in().rx(frame.bytes(), id);
			lcd.goto_xy(0,2);
			lcd << "CAN id:" << id << end;
			lcd.goto_xy(0,3);
			lcd << (frame.data()) << end;

			lcd.goto_xy(0,4);
			lcd << "RvRgArHd" << end;
			lcd.goto_xy(0,5);
			lcd << ((bool)reverse_switch.read()) << ((bool)regen_switch.read()) << ((bool)airgap_switch.read());
			lcd << ((bool)headlight_switch.read()) << end;

			if (id) led1.toggle();
			timer::delay_ms<250>();
		}

		static NORETURN void main();
	};
}

#endif /* defined(__nusolar_lib__driver_controls__) */

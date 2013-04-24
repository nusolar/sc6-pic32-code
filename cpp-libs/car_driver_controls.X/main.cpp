/*
 * File:   main.cpp
 * Author: alex
 *
 * Created on April 16, 2013, 2:47 AM
 */

#include "common_pragmas.h"
#include "compiler.h"
#include "enum.h"
#include "timer.h"
#include <cstdint>
#include <cstddef>

#include "nu32.h"
#include "nokia5110.h"
#include "can.h"
#include "spi.h"
#include "pinctl.h"
#include "wdt.h"

namespace nu {
	struct DriverControls: protected Nu32 {
		can::Module ws_can, common_can;
		Nokia5110 lcd;

		Enum<Pin, 3> analog_ins;
		#define ANALOG_INS\
			_PIN(regen_pedel, B, 0)\
			_PIN(accel_pedel, B, 1)\
			_PIN(airgap_pot, B, 4)

		Enum<Pin, 5> digital_ins;
		#define DIGITAL_INS\
			_PIN(brake_pedal,         B, 2)\
			_PIN(headlight_switch,    B, 3)\
			_PIN(airgap_enable,       B, 5)\
			_PIN(regen_enable,        B, 8)\
			_PIN(reverse_switch,      B, 9)

		Enum<Pin, 4> digital_outs;
		#define DIGITAL_OUTS\
			_PIN(lights_brake, D, 0)\
			_PIN(lights_l,	   D, 1)\
			_PIN(lights_r,     D, 2)\
			_PIN(headlights,   D, 3)

		/*
		 * Enumeration constants.
		 */
		#define _PIN(name, ltr, num) uint16_t name##_k; // FUCK MPLAB
			ANALOG_INS
			DIGITAL_INS
			DIGITAL_OUTS
		#undef _PIN

		/**
		 * State of DriverControls-relevant parameters.
		 * TODO: Implement extensible, polymorphic, car-wide state management.
		 */
		struct state {
			bool lights_head, lights_brake, lights_l, lights_r, lights_hazard,
				horn, // DISCONNECTED
				accel_en, brake_en, reverse_en, regen_en, airgap_en;
			float accel, regen, airgap;
			
			state(): lights_head(0), lights_brake(0), lights_l(0), lights_r(0), // FUCK MPLAB
				lights_hazard(0), horn(0), accel_en(0), brake_en(0), reverse_en(0),
				regen_en(0), airgap_en(0), accel(0), regen(0), airgap(0) {}
		} state;


		/**
		 * Setup CAN, input Pins, output Pins, and Nokia LCD.
		 */
		ALWAYSINLINE DriverControls(): Nu32(Nu32::V2), ws_can(CAN1), common_can(CAN2),
			lcd(SPI(Pin(IOPORT_G, BIT_9), SPI_CHANNEL2), Pin(IOPORT_A, BIT_9), Pin(IOPORT_E, BIT_9))
		{
			WDT::clear();
			#define _PIN(name, ltr, num) name##_k = _ITER.enumerate(Pin(IOPORT_##ltr, BIT_##num, #name));
				#define _ITER analog_ins
					ANALOG_INS
				#undef _ITER
				#define _ITER digital_ins
					DIGITAL_INS
				#undef _ITER
				#define _ITER digital_outs
					DIGITAL_OUTS
				#undef _ITER
			#undef _PIN

			common_can.setup();
			common_can.in()  = (can::RxChannel(can::Channel(common_can, CAN_CHANNEL0), CAN_RX_FULL_RECEIVE));
			common_can.out() = (can::TxChannel(can::Channel(common_can, CAN_CHANNEL1), CAN_HIGH_MEDIUM_PRIORITY));
			common_can.err() = (can::TxChannel(can::Channel(common_can, CAN_CHANNEL2), CAN_LOWEST_PRIORITY)); // err chn

			ws_can.setup();
			ws_can.out() = (can::TxChannel(can::Channel(ws_can, CAN_CHANNEL1), CAN_HIGH_MEDIUM_PRIORITY));

			// TODO: configure ADC10
			lcd.setup();
		}


		/**
		 * Read all input Pins. Store result in state.
		 */
		void ALWAYSINLINE read_ins() {
			WDT::clear();
			// TODO: Encapsulate ANALOG reading!
			state.accel = ((float)ReadADC10(1) + 0)/1024; // scale 0-1023 to 0-1
			if (state.accel < 0) state.accel = 0; // TODO: print warning, clamp
			if (state.accel > 1) state.accel = 1; // TODO: print warning
			state.accel_en = state.accel > 0.05;

			state.regen = ((float)ReadADC10(analog_ins[regen_pedel_k].num) + 0)/1024; // WARNING: disconnected
			state.regen_en = digital_ins[regen_enable_k].read()? 1: 0; // TODO: clamp

			state.airgap = ((float)ReadADC10(analog_ins[airgap_pot_k].num) + 0)/1024; // WARNING: disconnected
			state.airgap_en = digital_ins[airgap_enable_k].read()? 1: 0; // TODO: clamp

			state.reverse_en	= digital_ins[reverse_switch_k].read();

			state.brake_en		= digital_ins[brake_pedal_k].read()? 1: 0;
			state.lights_brake = state.brake_en;

			state.lights_head	= digital_ins[headlight_switch_k].read()? 1: 0;
		}


		/**
		 * Read car state from CAN.
		 */
		void ALWAYSINLINE recv_can() {
			char inc[8]; uint32_t id;
			common_can.in().rx(inc, id);
			switch (id) {
				case (uint32_t)can::addr::sw::tx::buttons_k: {
					can::frame::sw::tx::buttons btns = *(can::frame::sw::tx::buttons*)&inc;
					state.lights_l = btns.left;
					state.lights_r = btns.right;
					state.lights_hazard = btns.hazard;
					return;
				}
				default:
					return;
			}
		}


		/**
		 * Update all light Outputs, to conform to state.
		 */
		void ALWAYSINLINE set_lights() {
			WDT::clear();
			digital_outs[headlights_k]		= state.lights_head;
			digital_outs[lights_brake_k]	= state.lights_brake;
			
			bool tick = timer_s()%2;// Even or Odd, change every second
			digital_outs[lights_l_k] = state.lights_l||state.lights_hazard? tick: 0;
			digital_outs[lights_r_k] = state.lights_r||state.lights_hazard? tick: 0;
		}


		/**
		 * Command motor, from accel_pedel, brake, & cruise control input.
		 */
		void ALWAYSINLINE set_motor() {
			WDT::clear();
			can::frame::ws20::rx::drive_cmd drive {0, 0}; // [current, velocity]

			if (state.brake_en) {
				if (state.regen_en)
					drive = {0.2, 0}; // WARNING: REGEN_AMOUNT
				else
					drive = {0, 0};
			} else if (state.accel_en)
				drive = {state.accel, 100};

			if (state.reverse_en)
				drive.motorVelocity *= -1;

			led1.on(); delay_ms(100); led1.off(); // WARNING: WTF
			ws_can.out().tx(&drive, sizeof(drive), 0); // ERROR: CAN ADDRESS?
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
			lcd.printf("%f", 25);
		}


		/** Demo LED toggling */
		void ALWAYSINLINE demo() {
			WDT::clear();
			delay_ms(100);
			led1.toggle();
			led2.on();
		}
	};
}

using namespace std;
using namespace nu;

/**
 * Instantiate DriverControls object
 */
int main(int argc, const char* argv[]) {
	DriverControls dc{};
	while (true) {
		dc.demo();
	}
	return 0;
}


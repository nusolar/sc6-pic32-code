#include "nu/common_pragmas.h"

#include "nu/compiler.h"
#include "nupp/enum.hpp"
#include "nupp/timer.hpp"
#include <cstdint>
#include <cstddef>

#include "nupp/nu32.hpp"
#include "nupp/nokia5110.hpp"
#include "nupp/can.hpp"
#include "nupp/spi.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/wdt.hpp"

namespace nu {
	struct DriverControls: protected Nu32 {
		can::Module ws_can, common_can;
		Nokia5110 lcd;

		Enum<Pin, 3> analog_ins;
		#define ANALOG_INS(X)\
			X(regen_pedel,	B,	0)\
			X(accel_pedel,	B,	1)\
			X(airgap_pot,	B,	4)

		Enum<Pin, 5> digital_ins;
		#define DIGITAL_INS(X)\
			X(brake_pedal,		B,	2)\
			X(headlight_switch,	B,	3)\
			X(airgap_enable,	B,	5)\
			X(regen_enable,		B,	8)\
			X(reverse_switch,	B,	9)

		Enum<Pin, 4> digital_outs;
		#define DIGITAL_OUTS(X)\
			X(lights_brake,	D,	0)\
			X(lights_l,		D,	1)\
			X(lights_r,		D,	2)\
			X(headlights,	D,	3)

		/*
		 * Enumeration constants.
		 */
		#define DC_DECLARE(name, ltr, num) uint16_t name##_k; // FUCK MPLAB
		ANALOG_INS(DC_DECLARE)
		DIGITAL_INS(DC_DECLARE)
		DIGITAL_OUTS(DC_DECLARE)

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

			state(): lights_l(0), lights_r(0), lights_hazard(0),
				lights_head(0), lights_brake(0), horn(0),
				accel_en(0), brake_en(0), reverse_en(0), regen_en(0), airgap_en(0), cruise_en(0),
				accel(0), regen(0), airgap(0), cruise(0), sw_timer(0) {}
		} state;


		/**
		 * Setup CAN, input Pins, output Pins, and Nokia LCD.
		 */
		ALWAYSINLINE DriverControls(): Nu32(Nu32::V1), ws_can(CAN1), common_can(CAN2),
			lcd(SPI(Pin(IOPORT_G, BIT_9), SPI_CHANNEL2), Pin(IOPORT_A, BIT_9), Pin(IOPORT_E, BIT_9))
		{
			WDT::clear();
			#define DC_ENUMERATE(name, ltr, num, X_ITER) name##_k = X_ITER.enumerate(Pin(IOPORT_##ltr, BIT_##num, #name));
			#define DC_AIN(...)  DC_ENUMERATE(__VA_ARGS__, analog_ins)
			#define DC_DIN(...)  DC_ENUMERATE(__VA_ARGS__, digital_ins)
			#define DC_DOUT(...) DC_ENUMERATE(__VA_ARGS__, digital_outs)
			ANALOG_INS(DC_AIN)
			DIGITAL_INS(DC_DIN)
			DIGITAL_OUTS(DC_DOUT)

			common_can.setup();
			common_can.in()  = can::RxChannel(can::Channel(common_can, CAN_CHANNEL0), CAN_RX_FULL_RECEIVE);
			common_can.out() = can::TxChannel(can::Channel(common_can, CAN_CHANNEL1), CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err() = can::TxChannel(can::Channel(common_can, CAN_CHANNEL2), CAN_LOWEST_PRIORITY); // err chn

			ws_can.setup();
			ws_can.out() = can::TxChannel(can::Channel(ws_can, CAN_CHANNEL1), CAN_HIGH_MEDIUM_PRIORITY);

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
			uint32_t id;
			can::frame::Packet incoming;
			common_can.in().rx(incoming.bytes(), id);
			switch (id) {
				case (uint32_t)can::addr::sw::tx::buttons_k: {
					can::frame::sw::tx::buttons btns(incoming);
					state.lights_l = btns.frame.s.left;
					state.lights_r = btns.frame.s.right;
					state.lights_hazard = btns.frame.s.hazard;
					state.sw_timer = timer_ms();
					return;
				}
				default:
					uint32_t dc_time = timer_ms(); // Kill SW things if SW removed.
					if ((dc_time > state.sw_timer + 500) || ((dc_time < state.sw_timer) && (dc_time > 500))) {
						state.lights_l = 0;
						state.lights_r = 0;
						state.lights_hazard = 0;
						state.cruise_en = 0;
					}
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

			can::frame::ws20::rx::drive_cmd drive; // Zero-init [current, velocity]

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

			led1.on(); delay_ms(100); led1.off(); // WARNING: WTF
			ws_can.out().tx(drive.bytes(),
							8,
							(uint16_t)can::addr::ws20::rx::drive_cmd_k); // ERROR: CAN ADDRESS?
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
			lcd.printf("%f", 25.0);
		}


		/** Demo LED toggling */
		void ALWAYSINLINE demo() {
			WDT::clear();
			led1.toggle();
			led2.on();
			delay_ms(1000);
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


//
//  steering_wheel.h
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__steering_wheel__
#define __nusolar_lib__steering_wheel__

#include "nu/compiler.h" // MAXIMUM WARNING MAXIMUM ERROR WTF. MUST BE INCLUDED FIRST

#include <cstdio>
#include <bitset>
#include "nupp/ulcd28pt.hpp"
#include "nupp/enum.hpp"
#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"
#include "nupp/button.hpp"
#include "nupp/led.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"


// Pin definitions
#define SW_BTNS(_BTN)	\
	_BTN(yes,          E, 0)	\
	_BTN(no,           G, 12)	\
	_BTN(maybe,        E, 2)	\
	_BTN(cruise_en,    D, 0)	\
	_BTN(cruise_mode,  D, 8)	\
	_BTN(cruise_up,    D, 10)	\
	_BTN(cruise_down,  A, 15)
#define SW_LEDS(_LED)			\
	_LED(left,         D, 7)    \
	_LED(right,        D, 3)    \
	_LED(radio,        E, 5)    \
	_LED(yes,          E, 1)    \
	_LED(hazard,       D, 13)   \
	_LED(cruise_en,    D, 1)    \
	_LED(cruise_up,    D, 11)   \
	_LED(maybe,        E, 2)    \
	_LED(no,           G, 13)   \
	_LED(horn,         D, 5)    \
	_LED(cruise_mode,  D, 9)    \
	_LED(cruise_down,  A, 15)

#define SW_DECLARE_BTNS(name, ltr, num) size_t name##_k;
#define SW_DECLARE_LEDS(name, ltr, num) size_t led_##name##_k;
#define SW_INIT_BTNS(name, ltr, num) name##_k(buttons.enumerate(Button(Pin(Pin::ltr, num, #name), 10, 5))),
#define SW_INIT_LEDS(name, ltr, num) led_##name##_k(leds.enumerate(Led(Pin(Pin::ltr, num, #name)))),


namespace nu {
	struct SteeringWheel {
		SW_BTNS(SW_DECLARE_BTNS)
		SW_LEDS(SW_DECLARE_LEDS)

		can::Module common_can;
		uLCD28PT lcd;

		Enum<Button, 13> buttons;
		Enum<Led, 12> leds;

		/** State of Steering Wheel & car */
		struct state {
			std::bitset<13> btns, leds; // state of buttons & LEDs
			can::frame::sw::rx::lights lights; // requested LED state
			can::frame::ws20::tx::motor_velocity velo;
			can::frame::ws20::tx::current_vector curr;

			ALWAYSINLINE state(): btns(0), leds(0), lights(), velo(), curr() {}
		} state;


		/**
		 * Setup NU32, CAN, LEDs, and uLCD Display.
		 */
		ALWAYSINLINE SteeringWheel(): SW_BTNS(SW_INIT_BTNS) SW_LEDS(SW_INIT_LEDS)
			common_can(CAN2), lcd(UART3),
			buttons(), leds(), state()
		{
			WDT::clear();
			common_can.in()  = can::RxChannel(can::Channel(common_can, CAN_CHANNEL0), CAN_RX_FULL_RECEIVE);
			common_can.out() = can::TxChannel(can::Channel(common_can, CAN_CHANNEL1), CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err() = can::TxChannel(can::Channel(common_can, CAN_CHANNEL2), CAN_LOWEST_PRIORITY);
		}


		/**
		 * Animate LEDs on startup.
		 */
		void ALWAYSINLINE animate_leds() {
			// FUCK MPLAB
			for (unsigned i=0; i<leds.size(); i++){
				WDT::clear();
				leds[i].on();
				timer::delay_ms(100); // ok. WDT timeout ~ 2s
			}
			for (unsigned i=0; i<leds.size(); i++){
				WDT::clear();
				leds[i].off();
				timer::delay_ms(100); // ok. WDT timeout ~ 2s
			}
		}


		/**
		 * Read value of Button & LED Pins, update internal state.
		 */
		void ALWAYSINLINE read_ins() {
			WDT::clear();
			for (int repeat = 0; repeat < 10; repeat++) // re-update 10x
				for (unsigned i = 0; i < buttons.size(); i++)
					buttons[i].update();
			// WARNING: Should zero bitset?
			for (unsigned i = 0; i < buttons.size(); i++)
				state.btns[i] = (bool)buttons[i];
			for (unsigned i = 0; i < leds.size(); i++)
				state.leds[i] = (bool)leds[i];
		}


		/**
		 * Receive data to draw on LCD. Also receive any commands.
		 */
		void ALWAYSINLINE recv_can() {
			WDT::clear();
			can::frame::Packet incoming;
			uint32_t id;
			common_can.in().rx(incoming.bytes(), id);

			switch (id) {
				case (uint32_t)can::addr::sw::rx::lights_k:
					state.lights.data() = incoming.data();
					// WARNING: unimplemented
					break;
				case (uint32_t)can::addr::ws20::tx::motor_velocity_k:
					state.velo.data() = incoming.data();
					break;
				case (uint32_t)can::addr::ws20::tx::current_vector_k:
					state.curr.data() = incoming.data();
					break;
				default:
					break;
			}
		}


		/**
		 * Set each LED according to Steering Wheel's state.
		 */
		void ALWAYSINLINE set_leds() {
			for (unsigned i=0; i < leds.len; i++) {
				leds[i] = state.leds[i];
			}
		}


		/**
		 * Read the car's state from CAN, draw to dashboard LCD.
		 */
		void ALWAYSINLINE draw_lcd(){
			lcd << state.velo << state.curr;
			uint32_t msg = 0x004e4550;
			char *alert = NULL;
			sprintf(alert, "\x1Etext\x1F%s%s%u\x1E", "Alert: ", (char *)&msg, 0xF);
			lcd << alert;
		}


		/**
		 * Send CAN update of Steering Wheel status (LEDs and Buttons).
		 */
		void ALWAYSINLINE send_can() {
			WDT::clear();

			can::frame::sw::tx::buttons btns_pkt;
			btns_pkt.data() = state.btns.to_ullong();
			common_can.out().tx(btns_pkt.bytes(),
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::addr::sw::tx::buttons_k);

			can::frame::sw::tx::lights lts_pkt;
			lts_pkt.data() = state.leds.to_ullong();
			common_can.out().tx(lts_pkt.bytes(),
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::addr::sw::tx::lights_k);
		}

		/**
		 * A function to be called repeatedly.
		 */
		void ALWAYSINLINE run() {
			WDT::clear();

			read_ins(); //updates 10x, reads once
			recv_can(); //recv 1 packet

			set_leds();
			draw_lcd();

			send_can();
		}

		/** Demo LED toggling */
		void ALWAYSINLINE demo() {
			WDT::clear();
			leds[0].toggle();
			timer::delay_s(1);
		}

		static NORETURN void main();
	};
};

#endif /* defined(__nusolar_lib__steering_wheel__) */

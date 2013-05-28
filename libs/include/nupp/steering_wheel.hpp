#ifndef NUPP_STEERING_WHEEL_HPP
#define NUPP_STEERING_WHEEL_HPP 1

#include "nu/compiler.h"
#include "nupp/bitset.hpp"
#include "nupp/ulcd28pt.hpp" // MAXIMUM WARNING MAXIMUM ERROR MUST COME BEFORE PIC32
#include "nupp/button.hpp"
#include "nupp/led.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"
#include "nupp/timer.hpp"
#include "nupp/nu32.hpp"
#include <cstdio>

/* //Pin definitions. L/R/Haz/Horn are wired to MAYBE.
#define SW_BTNS(_BTN)	\
	_BTN(left,			D, 0)\
	_BTN(right,			D, 0)\
	_BTN(yes,			E, 0)	\
	_BTN(no,			G, 12)	\
	_BTN(maybe,			E, 2)	\
	_BTN(hazard,		D, 0)\
	_BTN(horn,			D, 0)\
	_BTN(cruise_en,		D, 0)	\
	_BTN(cruise_mode,	D, 8)	\
	_BTN(cruise_up,		D, 10)	\
	_BTN(cruise_down,	A, 15)

#define SW_BTNS(_BTN)	\
	_BTN(yes,			E, 0)	\
	_BTN(no,			G, 12)	\
	_BTN(maybe,			E, 2)	\
	_BTN(cruise_en,		D, 0)	\
	_BTN(cruise_mode,	D, 8)	\
	_BTN(cruise_up,		D, 10)	\
	_BTN(cruise_down,	A, 15)
#define SW_LEDS(_LED)			\
	_LED(left,			D, 7)   \
	_LED(right,			D, 3)   \
	_LED(radio,			E, 5)   \
	_LED(yes,			E, 1)   \
	_LED(hazard,		D, 13)  \
	_LED(cruise_en,		D, 1)   \
	_LED(cruise_up,		D, 11)  \
	_LED(maybe,			E, 3)   \
	_LED(no,			G, 13)  \
	_LED(horn,			D, 5)   \
	_LED(cruise_mode,	D, 9)   \
	_LED(cruise_down,	A, 15)*/

#define SW_N_BTNS 7
#define SW_N_LEDS 12

namespace nu {
	struct SteeringWheel {
		Button yes, no, maybe, cruise_en, cruise_mode, cruise_up, cruise_down;
		Led led_left, led_right, led_radio, led_yes, led_hazard, led_cruise_en,
			led_cruise_up, led_maybe, led_no, led_horn, led_cruise_mode, led_cruise_down;

		Button *buttons[SW_N_BTNS];
		Led *leds[SW_N_LEDS];
		can::Module common_can;
		uLCD28PT lcd;

		/** State of Steering Wheel & car */
		struct state {
			Bitset<13> btns, leds; // state of buttons & LEDs
			can::frame::sw::rx::lights lights; // requested LED state
			can::frame::ws20::tx::motor_velocity velo;
			can::frame::ws20::tx::current_vector curr;
			bool cc_en_on;
			bool cc_en_toggling;
			ALWAYSINLINE state(): btns(0), leds(0), lights(), velo(), curr(), 
				cc_en_on(0), cc_en_toggling(0) {}
		} state;


		/**
		 * Setup NU32, CAN, LEDs, and uLCD Display.
		 */
		ALWAYSINLINE SteeringWheel(): 
			yes			(Button(Pin(Pin::E,  0), 10, 5)),
			no			(Button(Pin(Pin::G, 12), 10, 5)),
			maybe		(Button(Pin(Pin::E,  2), 10, 5)),
			cruise_en	(Button(Pin(Pin::D,  0), 10, 5)),
			cruise_mode	(Button(Pin(Pin::D,  8), 10, 5)),
			cruise_up	(Button(Pin(Pin::D, 10), 10, 5)),
			cruise_down	(Button(Pin(Pin::A, 15), 10, 5)),

			led_left		(Led(Pin(Pin::D,  7))),
			led_right		(Led(Pin(Pin::D,  2))),
			led_radio		(Led(Pin(Pin::E,  5))),
			led_yes			(Led(Pin(Pin::E,  1))),
			led_hazard		(Led(Pin(Pin::D, 13))),
			led_cruise_en	(Led(Pin(Pin::D,  1))),
			led_cruise_up	(Led(Pin(Pin::D, 11))),
			led_maybe		(Led(Pin(Pin::E,  3))),
			led_no			(Led(Pin(Pin::G, 13))),
			led_horn		(Led(Pin(Pin::D,  5))),
			led_cruise_mode	(Led(Pin(Pin::D,  9))),
			led_cruise_down	(Led(Pin(Pin::A, 14))),

			buttons {&yes, &no, &maybe, &cruise_en, &cruise_mode, &cruise_up, &cruise_down},
			leds {&led_left, &led_right, &led_radio, &led_yes, &led_hazard,
				&led_cruise_en, &led_cruise_up, &led_maybe, &led_no, &led_horn,
				&led_cruise_mode, &led_cruise_down},

			common_can(CAN2), lcd(UART3), state()
		{
			
			WDT::clear();
			common_can.in().setup_rx();
			common_can.in().add_filter(CAN_FILTER0, CAN_SID, 0x400, CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7F8);
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);
		}


		/**
		 * Animate LEDs on startup.
		 */
		void ALWAYSINLINE animate_leds() {
			for (unsigned i=0; i<SW_N_LEDS; i++){
				WDT::clear();
				leds[i]->on();
				timer::delay_ms<100>(); // ok. WDT timeout ~ 2s
			}
			for (unsigned i=0; i<SW_N_LEDS; i++){
				WDT::clear();
				leds[i]->off();
				timer::delay_ms<100>(); // ok. WDT timeout ~ 2s
			}
		}

		/**
		 * Read value of Button & LED Pins, update internal state.
		 */
		void ALWAYSINLINE read_ins() {
#if 0
			WDT::clear();
			for (int repeat = 0; repeat < 10; repeat++) // re-update 10x
				for (unsigned i = 0; i < SW_N_BTNS; i++)
					buttons[i].update();
			// WARNING: Should zero bitset?
			for (unsigned i = 0; i < SW_N_BTNS; i++)
				state.btns[i] = (bool)buttons[i];
			for (unsigned i = 0; i < SW_N_LEDS; i++)
				state.leds[i] = (bool)leds[i];

			if (state.btns[cruise_en_k] != state.cc_en_toggling) {
				state.cc_en_toggling = (bool)cruise_en;
				if (state.cc_en_toggling == false) {
					state.cc_en_on = !state.cc_en_on; // Toggle value on button release
					state.btns[cruise_en_k] = state.cc_en_on; // Set cc_en
				}
			}
#endif
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
			for (unsigned i=0; i < SW_N_LEDS; i++) {
				*leds[i] = state.leds[i];
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
			btns_pkt.data() = (uint64_t) state.btns;
			common_can.out().tx(btns_pkt.bytes(),
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::addr::sw::tx::buttons_k);

			can::frame::sw::tx::lights lts_pkt;
			lts_pkt.data() = (uint64_t) state.leds;
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
			animate_leds();
		}

		static NORETURN void main();
	};
}

#endif /* defined(__nusolar_lib__steering_wheel__) */

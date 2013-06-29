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
	_BTN(left,			D, 6)\
	_BTN(right,			D, 3)\
	_BTN(yes,			E, 0)	\
	_BTN(no,			G, 12)	\
	_BTN(maybe,			E, 2)	\
	_BTN(hazard,		D, 12)\
	_BTN(horn,			D, 4)\
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
	_LED(right,			D, 2)   \
	_LED(radio,			E, 5)   \
	_LED(yes,			E, 1)   \
	_LED(hazard,		D, 13)  \
	_LED(cruise_en,		D, 1)   \
	_LED(cruise_up,		D, 11)  \
	_LED(maybe,			E, 3)   \
	_LED(no,			G, 13)  \
	_LED(horn,			D, 5)   \
	_LED(cruise_mode,	D, 9)   \
	_LED(cruise_down,	A, 14)*/

#define SW_N_BTNS 11
#define SW_N_LEDS 12

namespace nu {
	struct SteeringWheel {
		Button yes, no, maybe,
			cruise_en, cruise_mode, cruise_up, cruise_down,
			left, right, hazard, horn;
		Led led_yes, led_no, led_maybe,
			led_cruise_en, led_cruise_mode, led_cruise_up, led_cruise_down,
			led_left, led_right, led_hazard, led_horn, led_radio;

		Button *buttons[SW_N_BTNS];
		Led *leds[SW_N_LEDS];
		can::Module common_can;
		uLCD28PT lcd;

		/** State of Steering Wheel & car */
		struct state {
			Bitset<SW_N_BTNS> btns; // state of buttons & LEDs
			Bitset<SW_N_LEDS> leds;
			can::frame::sw::rx::buttons buttons;
			can::frame::sw::rx::lights lights; // requested LED state
			can::frame::ws20::tx::motor_velocity velo;
			can::frame::ws20::tx::current_vector curr;

			ALWAYSINLINE state(): btns(0), leds(0), buttons(), lights(), velo(), curr() {}
		} state;


		/**
		 * Setup NU32, CAN, LEDs, and uLCD Display.
		 */
		ALWAYSINLINE SteeringWheel():
#if 0
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
#else
			yes			(Button(Pin(Pin::E,  0), 10, 5)),
			no			(Button(Pin(Pin::G, 12), 10, 5)),
			maybe		(Button(Pin(Pin::E,  2), 10, 5)),
			cruise_en	(Button(Pin(Pin::D,  0), 10, 5)),
			cruise_mode	(Button(Pin(Pin::D,  8), 10, 5)),
			cruise_up	(Button(Pin(Pin::D, 10), 10, 5)),
			cruise_down	(Button(Pin(Pin::A, 15), 10, 5)),
			left		(Button(Pin(Pin::D,	 6), 10, 5)),
			right		(Button(Pin(Pin::D,  3), 10, 5)),
			hazard		(Button(Pin(Pin::D, 12), 10, 5)),
			horn		(Button(Pin(Pin::D, 4), 10, 5)),

			led_yes			(Led(Pin(Pin::E,  1))),
			led_no			(Led(Pin(Pin::G, 13))),
			led_maybe		(Led(Pin(Pin::E,  3))),
			led_cruise_en	(Led(Pin(Pin::D,  1))),
			led_cruise_mode	(Led(Pin(Pin::D,  9))),
			led_cruise_up	(Led(Pin(Pin::D, 11))),
			led_cruise_down	(Led(Pin(Pin::A, 14))),
			led_left		(Led(Pin(Pin::D,  7))),
			led_right		(Led(Pin(Pin::D,  2))),
			led_hazard		(Led(Pin(Pin::D, 13))),
			led_horn		(Led(Pin(Pin::D,  5))),
			led_radio		(Led(Pin(Pin::E,  5))),

#endif

			buttons {&yes, &no, &maybe,
					&cruise_en, &cruise_mode, &cruise_up, &cruise_down,
					&left, &right, &hazard, &horn},
			leds {&led_yes, &led_no, &led_maybe,
				&led_cruise_en, &led_cruise_mode, &led_cruise_up, &led_cruise_down,
				&led_left, &led_right, &led_hazard, &led_horn, &led_radio},

			common_can(CAN2), lcd(UART3), state()
		{

			WDT::clear();
			common_can.in ().setup_rx();
			common_can.in ().add_filter(CAN_FILTER0, CAN_SID, 0x300, CAN_FILTER_MASK0, CAN_FILTER_MASK_IDE_TYPE, 0x7F8);
			common_can.in1().setup_rx(); // Listen for MotorController
			common_can.in1().add_filter(CAN_FILTER1, CAN_SID, 0x403, CAN_FILTER_MASK1, CAN_FILTER_MASK_IDE_TYPE, 0x7F8);
			common_can.out().setup_tx(CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err().setup_tx(CAN_LOWEST_PRIORITY);
		}


		/**
		 * Animate LEDs on startup.
		 */
		ALWAYSINLINE void animate_leds() {
			for (unsigned i=0; i<SW_N_LEDS; i++){
				WDT::clear();
				leds[i]->on();
				timer::delay_ms<10>(); // ok. WDT timeout ~ 2s
			}
			for (unsigned i=0; i<SW_N_LEDS; i++){
				WDT::clear();
				leds[i]->off();
				timer::delay_ms<10>(); // ok. WDT timeout ~ 2s
			}
		}

		/**
		 * Read value of Button & LED Pins, update internal state.
		 */
		ALWAYSINLINE void read_ins() {
			WDT::clear();
			for (unsigned i = 0; i < SW_N_BTNS; i++)
				for (unsigned repeat = 0; repeat < 10; repeat++) // debounce 10x
					buttons[i]->update();

			for (unsigned i=0; i < SW_N_BTNS; i++)
				state.btns[i] |= buttons[i]->toggled();

			for (unsigned i=0; i < SW_N_LEDS; i++)
				state.leds[i] = (bool)*leds[i]; // WARNING: Should zero bitset?
		}

		/**
		 * Receive data to draw on LCD. Also receive any commands.
		 */
		ALWAYSINLINE void recv_can() {
			WDT::clear();
			can::frame::Packet incoming;
			uint32_t id;
			common_can.in().rx(incoming.bytes(), id);

			switch (id) {
				case (uint32_t)can::frame::sw::rx::buttons_k:
					state.buttons.data() = incoming.data();
					state.btns &= ~state.buttons.data();
					break;
				case (uint32_t)can::frame::sw::rx::lights_k:
					state.lights.data() = incoming.data();
					// WARNING: unimplemented
					break;
				case (uint32_t)can::frame::ws20::tx::motor_velocity_k:
					state.velo.data() = incoming.data();
					break;
				case (uint32_t)can::frame::ws20::tx::current_vector_k:
					state.curr.data() = incoming.data();
					break;
				default:
					break;
			}
		}

		/**
		 * Set each LED according to Steering Wheel's state.
		 */
		ALWAYSINLINE void set_leds() {
			WDT::clear();
			for (unsigned i=0; i < SW_N_LEDS; i++) {
				leds[i]->set(state.btns[i]);
			}
		}

		/**
		 * Read the car's state from CAN, draw to dashboard LCD.
		 */
		ALWAYSINLINE void draw_lcd(){
			WDT::clear();
			lcd << state.velo << state.curr;
			uint32_t msg = 0x004e4550;
			char alert[30] = {0};
			sprintf(alert, "\x1Etext\x1F%s%s%u\x1E", "Alert: ", (char *)&msg, 0xF);
			lcd << alert;
		}

		/**
		 * Send CAN update of Steering Wheel status (LEDs and Buttons).
		 */
		ALWAYSINLINE void send_can() {
			WDT::clear();

			can::frame::sw::tx::buttons btns_pkt;
			btns_pkt.data() = (uint64_t) state.btns;
			common_can.out().tx(btns_pkt.bytes(),
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::frame::sw::tx::buttons_k);

			can::frame::sw::tx::lights lts_pkt;
			lts_pkt.data() = (uint64_t) state.leds;
			common_can.out().tx(lts_pkt.bytes(),
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::frame::sw::tx::lights_k);
		}

		/**
		 * A function to be called repeatedly.
		 */
		ALWAYSINLINE void run() {
			WDT::clear();

			read_ins(); //updates 10x, reads once
//			recv_can(); //recv 1 packet

			set_leds();
			draw_lcd();

			send_can();
		}


		/** Demo LED toggling */
		ALWAYSINLINE void test() {
			WDT::clear();
			animate_leds();
			read_ins();
			can::frame::ws20::tx::motor_velocity x(0);
			x.frame.contents = {50, 50};
			lcd << x;
		}

		static NORETURN void main();
	};
}

#endif /* defined(__nusolar_lib__steering_wheel__) */

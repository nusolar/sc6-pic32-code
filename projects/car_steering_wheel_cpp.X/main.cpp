#include "nupp/common_pragmas.hpp"
#include <bitset>
#include <cstdlib>
#include "nupp/enum.hpp"
#include "nupp/timer.hpp"

#include "nupp/nu32.hpp"
#include "nupp/button.hpp"
#include "nupp/led.hpp"
#include "nupp/ulcd28pt.hpp"
#include "nupp/can.hpp"
#include "nupp/wdt.hpp"

#include <bitset>
#include <cstdlib>

namespace nu {
	struct SteeringWheel: protected Nu32 {
		can::Module common_can;
		uLCD28PT lcd;

		Enum<Button, 13> buttons;
		Enum<Led, 12> leds;

		/** State of Steering Wheel & car */
		struct state {
			std::bitset<32> btns, leds; // state of buttons & LEDs
			can::frame::sw::rx::lights lights; // requested LED state
			can::frame::ws20::tx::motor_velocity velo;
			can::frame::ws20::tx::current_vector curr;
		} state;

		/*
		 * Pin definitions
		 */
		#define DIGITAL_IN_PINS(_BTN)	\
			_BTN(yes,          E, 0)	\
			_BTN(no,           G, 12)	\
			_BTN(maybe,        E, 2)	\
			_BTN(cruise_en,    D, 0)	\
			_BTN(cruise_mode,  D, 8)	\
			_BTN(cruise_up,    D, 10)	\
			_BTN(cruise_down,  A, 15)
		#define LED_PINS(_LED)			\
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

		#define SW_DECLARE_BTNS(name, ltr, num) uint16_t name##_k;
		#define SW_DECLARE_LEDS(name, ltr, num) uint16_t led_##name##_k;
			DIGITAL_IN_PINS(SW_DECLARE_BTNS)
			LED_PINS(SW_DECLARE_LEDS)


		/**
		 * Setup NU32, CAN, LEDs, and uLCD Display.
		 */
		ALWAYSINLINE SteeringWheel(): Nu32(Nu32::V2), common_can(CAN2), lcd(UART3) {
			WDT::clear();
			#define SW_INIT_BTNS(name, ltr, num) name##_k = buttons.enumerate(Button(Pin(IOPORT_##ltr, BIT_##num, #name), 10, 5));
			#define SW_INIT_LEDS(name, ltr, num) led_##name##_k = leds.enumerate(Led(Pin(IOPORT_##ltr, BIT_##num, #name)));
				DIGITAL_IN_PINS(SW_INIT_BTNS)
				LED_PINS(SW_INIT_LEDS)

			for (unsigned i=0; i<leds.size(); i++)
				leds[i].setup();
			
			common_can.setup();
			common_can.in() = can::RxChannel(can::Channel(common_can, CAN_CHANNEL0), CAN_RX_FULL_RECEIVE);
			common_can.out() = can::TxChannel(can::Channel(common_can, CAN_CHANNEL1), CAN_HIGH_MEDIUM_PRIORITY);
			common_can.err() = can::TxChannel(can::Channel(common_can, CAN_CHANNEL2), CAN_LOWEST_PRIORITY);

			lcd.setup(115200, Serial::NOT_USE_UART_INTERRUPT, INT_PRIORITY_DISABLED, (UART_FIFO_MODE)0, (UART_LINE_CONTROL_MODE)0, (UART_CONFIGURATION)0, (UART_ENABLE_MODE)(UART_ENABLE|UART_RX|UART_TX));
		}


		/**
		 * Animate LEDs on startup.
		 */
		void ALWAYSINLINE animate_leds() {
			WDT::clear();
			// FUCK MPLAB
			for (unsigned i=0; i<leds.size(); i++){
				WDT::clear();
				leds[i].on();
				delay_ms(100); // ok. WDT timeout ~ 2s
			}
			WDT::clear();
			for (unsigned i=0; i<leds.size(); i++){
				WDT::clear();
				leds[i].off();
				delay_ms(100); // ok. WDT timeout ~ 2s
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
				state.btns[i] = buttons[i].pressed();
			for (unsigned i = 0; i < leds.size(); i++)
				state.leds[i] = leds[i].status();
		}


		/**
		 * Receive data to draw on LCD. Also receive any commands.
		 */
		void ALWAYSINLINE recv_can() {
			WDT::clear();
			can::frame::Packet incoming;
			uint32_t id;
			common_can.in().rx(incoming.frame.d, id);

			switch (id) {
				case (uint32_t)can::addr::sw::rx::lights_k:
					state.lights.frame.i = incoming.frame.i;
					// WARNING: unimplemented
					break;
				case (uint32_t)can::addr::ws20::tx::motor_velocity_k:
					state.velo.frame.i = incoming.frame.i;
					break;
				case (uint32_t)can::addr::ws20::tx::current_vector_k:
					state.curr.frame.i = incoming.frame.i;
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
			lcd.printf("\x1Etext\x1F%s%s%u\x1E", "Alert: ", (char *)&msg, 0xF);
		}


		/**
		 * Send CAN update of Steering Wheel status (LEDs and Buttons).
		 */
		void ALWAYSINLINE send_can() {
			WDT::clear();

			can::frame::sw::tx::buttons btns_pkt;
			btns_pkt.frame.i = state.btns.to_ullong();
			common_can.out().tx(btns_pkt.frame.d,
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::addr::sw::tx::buttons_k);

			can::frame::sw::tx::lights lts_pkt;
			lts_pkt.frame.i = state.leds.to_ullong();
			common_can.out().tx(lts_pkt.frame.d,
								4, // 64->32 ok. WARNING BIT ORDER?
								(uint16_t)can::addr::sw::tx::lights_k);
		}

		/**
		 * A function to be called repeatedly.
		 */
		void ALWAYSINLINE run() {
			WDT::clear();

			read_ins();
			recv_can();

			set_leds();
			draw_lcd();

			send_can();
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
using namespace can;

int main() {
	SteeringWheel sw{};
	sw.animate_leds();
	while (true) {
		sw.demo();
	}
}
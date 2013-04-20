//
//  main.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//


#include <bitset>
#include <cstdlib>
#include "array.h"
#include "timer.h"

#include "nu32.h"
#include "button.h"
#include "led.h"
#include "serial.h"
#include "can.h"
#include "wdt.h"

namespace nu {
	struct SteeringWheel: protected Nu32 {
		Serial display;
		can::CAN can;
		
		Enum<Button, 13> buttons;
		Enum<Led, 12> leds;
		
		/*
		 * Pin definitions
		 */
		#define DIGITAL_IN_PINS			\
			_BTN(yes,          E, 0)	\
			_BTN(no,           G, 12)	\
			_BTN(maybe,        E, 2)	\
			_BTN(cruise_en,    D, 0)	\
			_BTN(cruise_mode,  D, 8)	\
			_BTN(cruise_up,    D, 10)	\
			_BTN(cruise_down,  A, 15)
		#define LED_PINS                \
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
		
		#define _BTN(name, ltr, num) uint16_t name##_k;
		#define _LED(name, ltr, num) uint16_t led_##name##_k;
			DIGITAL_IN_PINS
			LED_PINS
		#undef _BTN
		#undef _LED
		
		SteeringWheel(): Nu32(Nu32::V2, HZ), can(CAN2), display(UART2)
		{
			#define _BTN(name, ltr, num) name##_k = buttons.enumerate(Button(IOPORT_##ltr, BIT_##num, 10, 5, #name));
			#define _LED(name, ltr, num) led_##name##_k = leds.enumerate(Led(IOPORT_##ltr, BIT_##num, #name));
				DIGITAL_IN_PINS
				LED_PINS
			#undef _LED
			#undef _BTN
			WDT::clear();
			for (int i=0; i<leds.size(); i++)
				leds[i].setup();
			can.setup_easy((CAN_MODULE_EVENT)0, INT_PRIORITY_DISABLED);
			can.add_rx(CAN_CHANNEL0, 32, CAN_RX_FULL_RECEIVE);
			can.add_tx(CAN_CHANNEL1, 32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY);
			can.add_tx(CAN_CHANNEL2, 32, CAN_TX_RTR_DISABLED, CAN_LOWEST_PRIORITY);
			// WARNING: setup display
		}
		
		void animate_leds() {
			WDT::clear();
			for (int i=0; i<leds.size(); i++){
				WDT::clear();
				leds[i].on();
				delay_ms(100); // WARNING: WDT time-out period?
			}
			WDT::clear();
			for (int i=0; i<leds.size(); i++){
				WDT::clear();
				leds[i].off();
				delay_ms(100); // WARNING: WDT time-out period?
			}
		}
		
		void run() {
			WDT::clear();
			for (int repeat = 0; repeat < 10; repeat++)
				for (int i = 0; i < buttons.size(); i++)
					buttons[i].update();
			
			std::bitset<32> bits;
			for (int i = 0; i < buttons.size(); i++)
				bits[i] = buttons[i].pressed();
			
			uint32_t temp = (uint32_t)bits.to_ullong(); // 64->32 ok. WARNING BIT ORDER?
			can::frame::sw::tx::buttons btns_frame = *(can::frame::sw::tx::buttons*)&temp;
			can.tx(&btns_frame, sizeof(btns_frame), 1);
			
			bits = 0;
			for (unsigned i = 0; i < leds.size(); i++)
				bits[i] = leds[i].status();
			
			temp = bits.to_ullong();
			can::frame::sw::tx::lights lts_frame = *(can::frame::sw::tx::lights*)&temp;
			can.tx(&lts_frame, sizeof(lts_frame), 1);
			
			char inc[8]; uint32_t id;
			can.rx(inc, id);
			if (id == (uint32_t)can::addr::ws20::tx::motor_velocity_k){
				can::frame::ws20::tx::motor_velocity pkt =
				*(can::frame::ws20::tx::motor_velocity *)&inc;
				display.printf("Speed [m/s]: %f", pkt.vehicleVelocity);
			}
		}
	};
}

using namespace std;
using namespace nu;

int main() {
	SteeringWheel sw{};
	sw.animate_leds();
	while (true) {
		sw.run();
	}
}

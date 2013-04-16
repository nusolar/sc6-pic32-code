//
//  main.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nu32.h"
#include "button.h"
#include "led.h"
#include "serial.h"
#include "can.h"

#include <algorithm>
#include <bitset> //template <size_t N> class bitset;
#include <stdlib.h>
#include <vector>

namespace nu {
	struct SteeringWheel: protected Nu32 {
		Serial display;
		can::CAN can;
		
		std::vector<Button> buttons;
		std::vector<Led> leds;
		
		SteeringWheel();
		
		/*
		 * Gross field generation:
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
		
#define _BTN(name, ltr, num) \
Button name; /*= Button(IOPORT_##ltr, BIT_##num, 10, 5, #name);*/
		DIGITAL_IN_PINS
#undef _BTN
#define _LED(name, ltr, num) \
Led led_##name; /*= Led(IOPORT_##ltr, BIT_##num, #name);*/
		LED_PINS
#undef _LED
	};
}

using namespace std;
using namespace nu;

// stupid MPLAB won't initialize fields in-declaration like C++11 says, so:
#define _BTN(name, ltr, num) ,name(IOPORT_##ltr, BIT_##num, 10, 5, #name)
#define _LED(name, ltr, num) ,led_##name(IOPORT_##ltr, BIT_##num, #name)
SteeringWheel::SteeringWheel(): Nu32(Nu32::V2, HZ), display(UART2), can(CAN2)
	DIGITAL_IN_PINS
	LED_PINS
#undef _BTN
#undef _LED
{
	#define _BTN(name, ltr, num) \
		buttons.push_back(name);
		DIGITAL_IN_PINS
	#undef _BTN
	#define _LED(name, ltr, num) \
		leds.push_back(led_##name);
		LED_PINS
	#undef _LED

	for_each(leds.begin(), leds.end(), [](Led &x){
		x.setup();
	});

	while (1) {
		for_each(buttons.begin(), buttons.end(), [](Button &x){
			x.update();
		});

		bitset<32> bits;
		for (unsigned i = 0; i < buttons.size(); i++)
			bits[i] = buttons[i].pressed();
		
		uint32_t temp = (uint32_t)bits.to_ullong(); // WARNING BIT ORDER?
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
			can::frame::ws20::tx::motor_velocity pkt = *(can::frame::ws20::tx::motor_velocity *)&inc;
			display.printf("Speed [m/s]: %f", pkt.vehicleVelocity);
		}
	}
}

int main() {
	SteeringWheel sw{};
}

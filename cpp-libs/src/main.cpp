//
//  main.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "main.h"

#include <algorithm>
#include <bitset>

using namespace std;
using namespace nu;

//template <size_t N> class bitset;

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

		display.tx("Speed: 5 mph", sizeof(char));
	}
}

int main() {
	SteeringWheel sw{};
}

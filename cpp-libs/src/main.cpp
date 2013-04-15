//
//  main.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "main.h"

#include <algorithm>

using namespace nu;
using namespace std;

#define _BTN(name, ltr, num) ,name(IOPORT_##ltr, BIT_##num, 10, 5, "name")
#define _LED(name, ltr, num) ,led_##name(IOPORT_##ltr, BIT_##num, "name")
SteeringWheel::SteeringWheel(): Nu32(Nu32::V2, HZ), display(UART2)
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
		}); // should repeat?

		bool can_packet[buttons.size()];
		for (int i = 0; i < buttons.size(); i++)
			can_packet[i] = buttons[i].pressed();
		// SEND

		bool can_packet_led[leds.size()];
		for (int i = 0; i < leds.size(); i++)
			can_packet_led[i] = leds[i].status();
		// SEND


		// RECV
		display.tx("Speed: 5mph", sizeof(char));
	}
}

int main() {
	SteeringWheel sw{};
}

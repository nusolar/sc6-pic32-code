//
//  main.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__main__
#define __nusolar_lib__main__

#include "nu32.h"
#include "button.h"
#include "led.h"
#include "serial.h"

#include <vector>

namespace nu {
	struct SteeringWheel: protected Nu32 {
		Serial display;
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
			Button name;
			DIGITAL_IN_PINS
		#undef _BTN
		#define _LED(name, ltr, num) \
			Led led_##name;
			LED_PINS
		#undef _LED
	};
}

#endif /* defined(__nusolar_lib__main__) */

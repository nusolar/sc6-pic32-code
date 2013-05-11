#ifndef NUPP_BUTTON_HPP
#define NUPP_BUTTON_HPP 1

/**
 * @file
 * Wrapper for buttons, with debouncing.
 */

#include "nupp/pinctl.hpp"

namespace nu {
	/**
	 * Digital Button on one pin. Button contacts are flimsy, so
	 * call update() often to "debounce" the value.
	 *
	 */
	class Button: protected DigitalIn {
		int32_t debounce;
		uint8_t debounce_max;
		uint8_t thresh;

	public:
		ALWAYSINLINE Button(Pin btn = Pin(), uint8_t _debounce_max = 10, uint8_t _thresh = 5):
			DigitalIn(Pin(btn)), debounce(0), debounce_max(_debounce_max), thresh(_thresh) {}

		/** Get whether button is pressed, according to update()'s counter. */
		ALWAYSINLINE bool pressed()		{return debounce >= thresh;}
		ALWAYSINLINE operator bool()	{return debounce >= thresh;}

		/**
		 * CallMe often relative to threshold to maintain current button value.
		 */
		ALWAYSINLINE void update() {
			debounce += (read()? 1: -1);
			if (debounce < 0) debounce = 0;
			if (debounce > debounce_max) debounce = debounce_max;
		}
	};
}

#endif

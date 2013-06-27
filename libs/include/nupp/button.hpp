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
		bool was_pressed;

	public:
		ALWAYSINLINE Button(Pin btn = Pin(), uint8_t _debounce_max = 10, uint8_t _thresh = 5):
			DigitalIn(Pin(btn)), debounce(0), debounce_max(_debounce_max), thresh(_thresh), was_pressed(false) {}

		/** Get whether button is pressed, according to update()'s counter. */
		ALWAYSINLINE bool pressed()		{return debounce >= thresh;}
		ALWAYSINLINE operator bool()	{return debounce >= thresh;}

		/**
		 * For non-interrupt buttons, call OFTEN to debounce button value.
		 */
		ALWAYSINLINE void update() {
			debounce += (read()? 1: -1);
			if (debounce < 0) debounce = 0;
			if (debounce > debounce_max) debounce = debounce_max;
		}

		/**
		 * Returns whether button was released since last call. Call at least
		 * once while button is pressed, then once while released;
         * @warning Interrupts
         */
		ALWAYSINLINE bool toggled() {
			if (was_pressed != pressed()) {
				return !(was_pressed = !was_pressed);
			}
			return false;
		}
	};
}

#endif

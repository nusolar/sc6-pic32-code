#ifndef NUPP_BUTTON_HPP
#define NUPP_BUTTON_HPP 1

/**
 * @file
 * Wrapper for buttons, with debouncing.
 */

#include "nupp/pinctl.hpp"

namespace nu {
	/**
	 * HW Button with debouncing.
	 */
	struct Button: protected Pin {
		int32_t debounce;
		uint8_t debounce_max;
		uint8_t thresh;


		ALWAYSINLINE Button(Pin btn = Pin(), uint8_t _debounce_max = 10, uint8_t _thresh = 5):
			Pin(btn), debounce(0), debounce_max(_debounce_max), thresh(_thresh) {}

		bool ALWAYSINLINE pressed() {
			return debounce >= thresh;
		}
		void ALWAYSINLINE update() {
			debounce += (read()? 1: -1);
			if (debounce < 0) debounce = 0;
			if (debounce > debounce_max) debounce = debounce_max;
		}
	};
}

#endif

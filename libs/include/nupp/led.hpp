#ifndef NUPP_LED_HPP
#define NUPP_LED_HPP 1

/**
 * @file
 * Little wrapper for LEDs
 */

#include "nupp/pinctl.hpp"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	struct Led: protected DigitalOut {
		bool _status;
		
		ALWAYSINLINE Led(Pin led = Pin()): DigitalOut(Pin(led)), _status(false) {
			set_digital_out();
			off();
		}
		void ALWAYSINLINE off() {set(); _status = false;}
		void ALWAYSINLINE on() {clear(); _status = true;}
		void ALWAYSINLINE toggle() {Pin::toggle(); _status = !_status;}
		bool ALWAYSINLINE status() {return _status;}

		/**
		 * Alias to Led::status(), which may go away soon.
		 */
		ALWAYSINLINE operator bool() {return _status;}
		/**
		 * Turn LED [on/off] by assigning it to [true/false] respectively.
		 */
		ALWAYSINLINE Led& operator= (bool rhs) {
			if (rhs) on();
			else off();
			return *this;
		}
	};
}

#endif

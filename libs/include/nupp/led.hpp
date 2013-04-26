#ifndef NUPP_LED_HPP
#define NUPP_LED_HPP 1

#include "nupp/pinctl.hpp"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	struct Led: protected Pin {
		ALWAYSINLINE Led(Pin led = Pin()): Pin(led) {}

		void ALWAYSINLINE off() {set();}
		void ALWAYSINLINE on() {clear();}
		void ALWAYSINLINE toggle() {Pin::toggle();}
		unsigned int ALWAYSINLINE status() {return read();}
		
		/**
		 * Turn LED [on/off] by assigning it to [true/false] respectively.
		 */
		ALWAYSINLINE Led& operator= (bool rhs) {
			if (rhs)
				on();
			else
				off();
			return *this;
		}
		
		void ALWAYSINLINE setup() {
			set_digital_out();
			off();
		}
	};
}

#endif

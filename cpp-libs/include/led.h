#ifndef NUSOLAR_CPP_LED_H
#define NUSOLAR_CPP_LED_H 1

#include "pinctl.h"

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

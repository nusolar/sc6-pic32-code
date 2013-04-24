#ifndef NUPP_ULCD28PT_HPP
#define NUPP_ULCD28PT_HPP 1

#include <cstdint>
#include <cstddef>

#include "nupp/serial.hpp"
#include "nupp/pinctl.hpp"

namespace nu {
	/**
	 * The touch-sensitive LCD on the Steering Wheel.
	 */
	struct uLCD28PT: public Serial {
		
		ALWAYSINLINE uLCD28PT(UART_MODULE mod): Serial(mod) {}
		
	};
}

#endif

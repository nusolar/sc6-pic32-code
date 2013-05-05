#ifndef NUPP_NU32_HPP
#define NUPP_NU32_HPP 1

/**
 * @file
 * Common variables and utilities for Nick Marchuck's NU32 boards.
 *
 * Supports NU32v1 and NU32v2
 *
 * @sa http://hades.mech.northwestern.edu/index.php/NU32:_Introduction_to_the_PIC32
 */

#include "nupp/param.hpp"

#include "nupp/led.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/serial.hpp"
#include <plib.h>

namespace nu {
	struct Nu32 {
		enum versions {
			V1,
			V2
		};
		versions version;

		#define LIST_OF_PINS_V1(X) \
			X(Led, led1, Pin(Pin::A, 4)) \
			X(Led, led2, Pin(Pin::A, 5)) \
			X(DigitalIn, switch1, Pin(Pin::C, 13)) \
			X(Serial, serial1, UART1, 115200) \
			X(Serial, serial2, UART4, 115200) \

		#define LIST_OF_PINS_V2(X) \
			X(Led, led1, Pin(Pin::G, 12)) \
			X(Led, led2, Pin(Pin::G, 13)) \
			X(DigitalIn, switch1, Pin(Pin::G, 6)) \
			X(Serial, serial1, UART1, 115200) \
			X(Serial, serial2, UART3, 115200) \

		// FUCK MPLAB
		#define NU32_DECLARE(Type, name, ...) \
			Type name;
			LIST_OF_PINS_V1(NU32_DECLARE)

		/**
		 * Primary Setup. NECESSARILY call this before anything else!
		 * Do all setups from derived class's constructor!
		 */
		Nu32(versions _version, uint32_t _hz = param::default_hz());
	};
}

#endif /* defined(__nusolar_lib__nu32__) */

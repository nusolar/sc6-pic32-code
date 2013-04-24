#ifndef NUPP_NU32_HPP
#define NUPP_NU32_HPP 1

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
		
		#define LIST_OF_PINS_V1 \
			X(Led, led1, Pin(IOPORT_A, BIT_4)) \
			X(Led, led2, Pin(IOPORT_A, BIT_5)) \
			X(Pin, switch1, IOPORT_C, BIT_13) \
			X(Serial, serial1, UART1) \
			X(Serial, serial2, UART4) \

		#define LIST_OF_PINS_V2 \
			X(Led, led1, Pin(IOPORT_G, BIT_12)) \
			X(Led, led2, Pin(IOPORT_G, BIT_13)) \
			X(Pin, switch1, IOPORT_G, BIT_6) \
			X(Serial, serial1, UART1) \
			X(Serial, serial2, UART3) \
				
		// FUCK MPLAB
		#define X(Type, name, ...) \
			Type name;
			LIST_OF_PINS_V1
		#undef X
		
		/**
		 * Primary Setup. SuperConstructor-call this before ANY setup calls!
		 * Call all setups from derived class's constructor!
		 */
		Nu32(versions _version, uint64_t _hz = HZ);
	};
}

#endif /* defined(__nusolar_lib__nu32__) */

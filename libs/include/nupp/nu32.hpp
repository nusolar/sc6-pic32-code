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
	struct Nu32Init {
		Nu32Init(uint32_t _hz);
		virtual ~Nu32Init() {}
	};

	struct Nu32: public Nu32Init {
		enum V1_t {V1};
		enum V2_t {V2};
		enum versions {
			v1,
			v2
		} version;

		Led led1, led2;
		DigitalIn switch1;
		Serial serial1, serial2;

		/**
		 * NECESSARILY call this before anything else!
		 * Do all setups from derived class's constructor!
		 * Pass V1 for Nu32v1, V2 for Nu32v2.
		 */
		Nu32(V1_t, uint32_t _hz = param::default_hz()): Nu32Init(_hz), version(v1),
			led1(Pin(Pin::A, 4)), 
			led2(Pin(Pin::A, 5)),
			switch1(Pin(Pin::C, 13)),
			serial1(UART1, 115200),
			serial2(UART4, 115200) {}
		
		Nu32(V2_t, uint32_t _hz = param::default_hz()): Nu32Init(_hz), version(v2),
			led1(Pin(Pin::G, 12)),
			led2(Pin(Pin::G, 13)),
			switch1(Pin(Pin::G, 6)),
			serial1(UART1, 115200),
			serial2(UART3, 115200) {}
		
		virtual ~Nu32() {}
	};
}

#endif /* defined(__nusolar_lib__nu32__) */

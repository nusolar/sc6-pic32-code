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
extern "C" {
#include <plib.h>
}

namespace nu {
	struct Nu32Init {
		Nu32Init(uint32_t _hz);
		virtual ~Nu32Init() {}
	};

	/**
	 * NU32 is the Northwestern University PIC32 development board.
	 *
	 * led1 is the "L1" LED
	 * led2 is the "L2" LED
	 * switch1 is the User button.
	 * serial1 is for general communication.
	 * serial2 is for programming the Nu32 with Nick Marchuck's utility.
	 */
	struct Nu32: public Nu32Init {
		enum V2010_t {V2010};
		enum V2011_t {V2011};
		enum versions {
			v2010,
			v2011
		} version;

		Led led1, led2;
		DigitalIn switch1;
		Serial serial1, serial2;

		/**
		 * NECESSARILY call this before anything else!
		 * Do all setups from derived class's constructor!
		 * Pass V2010 for Nu32v10, V2011 for Nu32v2011.
		 */
		Nu32(V2010_t, uint32_t _hz = param::default_hz()): Nu32Init(_hz), version(v2010),
			led1(PIN(G, 12)),
			led2(PIN(G, 13)),
			switch1(PIN(G, 6)),
			serial1(UART(1)),
			serial2(UART(3)) {}
		
		Nu32(V2011_t, uint32_t _hz = param::default_hz()): Nu32Init(_hz), version(v2011),
			led1(PIN(A, 4)),
			led2(PIN(A, 5)),
			switch1(PIN(C, 13)),
			serial1(UART(1)),
			serial2(UART(3)) {}

		virtual ~Nu32() {}
	};
}

#endif /* defined(__nusolar_lib__nu32__) */

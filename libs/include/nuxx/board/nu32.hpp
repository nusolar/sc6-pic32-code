#ifndef NUXX_NU32_HPP
#define NUXX_NU32_HPP 1

/**
 * @file
 * Common variables and utilities for Nick Marchuck's NU32 boards.
 *
 * Supports NU32v1 and NU32v2
 *
 * @sa http://hades.mech.northwestern.edu/index.php/NU32:_Introduction_to_the_PIC32
 */


#include "nuxx/component/led.hpp"
#include "nuxx/component/button.hpp"
#include "nuxx/peripheral/serial.hpp"
#include "nuxx/peripheral/pin.hpp"
#include "nuxx/param.hpp"
#include <cstdint>

namespace nu {
	/**
	 * NU32 is the Northwestern University PIC32 development board.
	 *
	 * led1 is the "L1" LED
	 * led2 is the "L2" LED
	 * switch1 is the User button.
	 * serial1 is for general communication.
	 * serial2 is for programming the Nu32 with Nick Marchuck's utility.
	 */
	struct Nu32 {
		enum V2010_t {V2010};
		enum V2011_t {V2011};

		enum versions {
			v2010,
			v2011
		} version;

		Led led1, led2;
		Button switch1;
		Serial serial_usb1, serial_usb2;

		/**
		 * NECESSARILY call this before anything else!
		 * Do all setups from derived class's constructor!
		 * Pass V2010 for Nu32v10, V2011 for Nu32v2011.
		 */
		Nu32(V2010_t): version(v2010),
			led1(PIN(G, 12)),
			led2(PIN(G, 13)),
			switch1(PIN(G, 6)),
			serial_usb1(UART(1)),
			serial_usb2(UART(3)) {}

		Nu32(V2011_t): version(v2011),
			led1(PIN(A, 4)),
			led2(PIN(A, 5)),
			switch1(PIN(C, 13)),
			serial_usb1(UART(1)),
			serial_usb2(UART(3)) {}

		virtual ~Nu32() {}

		void setup()
		{
			this->led1.setup();
			this->led2.setup();
			this->switch1.setup();
			this->serial_usb1.setup();
			this->serial_usb2.setup();
		}
	};
}

#endif /* defined(__nusolar_lib__nu32__) */

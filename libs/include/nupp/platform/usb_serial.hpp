/* 
 * File:   usb_serial.hpp
 * Author: alex
 *
 * Created on July 7, 2013, 3:49 PM
 */

#ifndef NU_USB_SERIAL_HPP
#define	NU_USB_SERIAL_HPP

#include "nu/platform/usb_serial.h"
#include "nu/compiler.h"

#if PLATFORM==TEENSY /* USBSerial code for teensy */

namespace nu {
	struct USBSerial {
		USBSerial() {
			usb_init();
		}

		/** returns -1 if nothing is available */
		ALWAYSINLINE int16_t rx_char() {
			if (usb_serial_available())
				return usb_serial_getchar();
			return -1;
		}

		ALWAYSINLINE void flush_input() {
			usb_serial_flush_input();
		}

		ALWAYSINLINE int8_t putc(uint8_t c) {
			return usb_serial_putchar_nowait(c);
		}
	};
}
#endif /* Platform code */

#endif	/* NU_USB_SERIAL_HPP */


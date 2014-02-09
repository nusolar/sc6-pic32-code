/**
 * File:   usbhid.hpp
 * Author: alex
 *
 * Created on September 8, 2013, 3:14 PM
 *
 * @file This header depends on its implementation in the usb.X library!!!
 */

#ifndef NUXX_USBHID_HPP
#define	NUXX_USBHID_HPP

#include "nu/compiler.h"
#include <cstddef>

namespace nu {
	/**
	 * Interface to USB communication, as an HID Custom Class. Allows 64 bytes/ms,
	 * bidirectional. Construct ONE UsbHid object, and call try_rx()/try_tx()
	 * At Least every 40ms, PREFERABLY every 1ms to 10ms.
	 * Class is implemented in the usb.X library, for Microchip PIC32s ONLY.
	 */
	class UsbHid {
		unsigned char ReceivedDataBuffer[64];
		unsigned char ToSendDataBuffer[64];

		void *USBOutHandle;    //USB handle.  Must be initialized to 0 at startup.
		void *USBInHandle;

	public:
		/**
		 * Initializes USB stack, and attachs device if USB_INTERRUPT mode enabled.
		 * HID devices are USB_INTERRUPT devices, so it's enabled.
         */
		UsbHid();

		/**
		 * This method checks if a packet was received, and, if so, calls the
		 * callback and passes it a pointer to the buffer, and the number of
		 * bytes in the buffer.
         * @param callback The callback, which takes a pointer and a size_t
         */
		void try_rx(void (*callback)(unsigned char *, size_t len));

		/**
		 * This method checks if we can send a packet, and, if so, calls the
		 * callback and passes it a pointer to the TX buffer, and the number of
		 * bytes in the buffer.
         * @param callback The callback, which takes a pointer and a size_t
         */
		void try_tx(void (*callback)(unsigned char *, size_t len));
	};
}

#endif	/* NUXX_USBHID_HPP */


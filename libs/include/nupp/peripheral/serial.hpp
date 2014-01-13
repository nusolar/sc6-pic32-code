#ifndef NUPP_SERIAL_HPP
#define NUPP_SERIAL_HPP 1

#include "nu/compiler.h"
#include "nupp/stream.hpp"
#include "nupp/platform/serial.hpp"
#include <cstdint>
#include <cstring> // strlen
//#include <cstdio> /* Serial::printf requires these */
//#include <cstdarg>

#define TTY_UNIT '\x1F' // TODO static variable
#define TTY_RECORD '\x1E'

namespace nu {
	/**
	 * Encapsulate Serial/UART reading/writing.
	 */
	struct Serial: public OStream, protected UARTModule {
		Serial(UARTModule _module): OStream(), UARTModule(_module) {}
		virtual ~Serial() {}

		INLINE int rx(void *dst, size_t n)		{return UARTModule::rx(dst, n);}
		INLINE void tx(const void *src, size_t n)	{UARTModule::tx(src, n);}
		
		/** Required to implement OStream: */
		INLINE virtual void puts(const char *str) {
			tx(str, strlen(str));
		}

		/** Returns # of bytes copied. */
		INLINE size_t read_line(void *dst, size_t n) {
			for (size_t ui=0; ui < n; ++ui) {
				uint8_t buffer = 0;
				int error = UARTModule::rx(&buffer, 1);

				if (error < 0) {
					// Data unavailable, return # of bytes copied thusfar, EXCLUDING this final attempt.
					return ui;
				} else {
					// write byte to destination;
					((uint8_t *)dst)[ui] = buffer;
				}

				if (buffer == '\n') {
					// ENDL, return # of bytes copied, INCLUDING this attempt
					return ui+1;
				}
			}
			return n;
		}
	};
}

#endif

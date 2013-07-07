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

		ALWAYSINLINE void rx(void *dst, size_t n)		{UARTModule::rx(dst, n);}
		ALWAYSINLINE void tx(const void *src, size_t n)	{UARTModule::tx(src, n);}
		INLINE virtual void puts(const char *str) {
			tx(str, strlen(str));
		}
	};
}

#endif

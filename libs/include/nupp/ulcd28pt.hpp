#ifndef NUPP_ULCD28PT_HPP
#define NUPP_ULCD28PT_HPP 1

#include "nu/compiler.h"
#include <cstdint>
#include <cstddef>

#include <sstream>
#include "nupp/serial.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/can.hpp"

namespace nu {
	/**
	 * The touch-sensitive LCD on the Steering Wheel.
	 * The communication protocol is: "|text:value|",
	 * where port ('|') is the record separator ('\x1E')
	 * and colon (':') is the unit separator ('\x1F').
	 * The key, "text", must be 4 letters.
	 */
	class uLCD28PT: protected Serial {
		static const uint8_t unit = '\x1F';
		static const uint8_t record = '\x1E';
		static const uint16_t buffersize = 512;
		/**
		 * INITIALLY USES HEAP. HOWEVER, CONSTRUCTOR IMMEDIATELY
		 * GIVES IT STACK BUFFER.
		 */
//		std::stringstream s;
		char _buffer[buffersize];
		
		template <class V>
		ALWAYSINLINE void write_key_val(const char *key, V &value) {
//			s << record << key << unit << value << record;
//			Serial::puts(s.str().c_str());
//			s.str("");
		}

	public:
		ALWAYSINLINE uLCD28PT(UART_MODULE mod): Serial(mod, 115200)//, s()
		{
//			s.rdbuf()->pubsetbuf(_buffer, buffersize);
		}

		ALWAYSINLINE uLCD28PT& operator << (const can::frame::ws20::tx::motor_velocity& x) {
			write_key_val("velo", x.frame.s.vehicleVelocity);
			return *this;
		}
		ALWAYSINLINE uLCD28PT& operator << (const can::frame::ws20::tx::current_vector& x) {
			write_key_val("curr", x.frame.s.currentRe);
			return *this;
		}
		ALWAYSINLINE uLCD28PT& operator << (const char * x) {
			write_key_val("text", x);
			return *this;
		}
	};
}

#endif

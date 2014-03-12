#ifndef NUXX_ULCD28PT_HPP
#define NUXX_ULCD28PT_HPP 1

#include "nuxx/peripheral/serial.hpp"
#include "nuxx/peripheral/pin.hpp"
#include "nuxx/peripheral/can.hpp"
#include "nu/compiler.h"
#include <cstdint>
#include <cstddef>

#define TTY_UNIT '\x1F'
#define TTY_RECORD '\x1E'

namespace nu {
	/**
	 * The touch-sensitive LCD on the Steering Wheel.
	 * The communication protocol takes the format: "|text:value|",
	 * where port ('|') is the record separator ('\x1E')
	 * and colon (':') is the unit separator ('\x1F').
	 * The key, "text", must be 4 letters.
	 */
	class uLCD28PT {
//		template <class V>
//		void write_key_val(const char *key, V value) {
//			base.printf("%c%s%c%f%c", (TTY_RECORD), (key), (TTY_UNIT), (value), (TTY_RECORD));
//		}
		void write_key_val(const char *key, float value) {
			base.printf("%c%s%c%f%c", (TTY_RECORD), (key), (TTY_UNIT), (value), (TTY_RECORD));
		}
		void write_key_val(const char *key, const char * value) {
			base.printf("%c%s%c%s%c", (TTY_RECORD), (key), (TTY_UNIT), (value), (TTY_RECORD));
		}

	public:
		Serial base;

		uLCD28PT(PlatformUart mod): base(mod) {}
		void setup()	{this->base.setup();}

		uLCD28PT& operator << (const Can::Addr::ws20::motor_velocity& x) {
			write_key_val("velo", x.frame().vehicleVelocity);
			return *this;
		}
		uLCD28PT& operator << (const Can::Addr::ws20::current_vector& x) {
			write_key_val("curr", x.frame().currentRe);
			return *this;
		}
		uLCD28PT& operator << (const char * x) {
			write_key_val("text", x);
			return *this;
		}
	};
}

#endif

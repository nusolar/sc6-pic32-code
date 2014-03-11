#ifndef NUXX_ULCD28PT_HPP
#define NUXX_ULCD28PT_HPP 1

#include "nuxx/peripheral/serial.hpp"
#include "nuxx/peripheral/pinctl.hpp"
#include "nuxx/peripheral/can.hpp"
#include "nu/compiler.h"
#include <cstdint>
#include <cstddef>

namespace nu {
	/**
	 * The touch-sensitive LCD on the Steering Wheel.
	 * The communication protocol is: "|text:value|",
	 * where port ('|') is the record separator ('\x1E')
	 * and colon (':') is the unit separator ('\x1F').
	 * The key, "text", must be 4 letters.
	 */
	class uLCD28PT: protected Serial {
		template <class V>
		void write_key_val(const char *key, V value) {
			OStream::operator<< (TTY_RECORD) << (key) <<  (TTY_UNIT) << (value) << (TTY_RECORD) << (end);
		}

	public:
		uLCD28PT(UARTModule mod): Serial(mod) {}

		uLCD28PT& operator << (const Can::Addr::ws20::motor_velocity& x) {
			write_key_val<float>("velo", x.frame().vehicleVelocity);
			return *this;
		}
		uLCD28PT& operator << (const Can::Addr::ws20::current_vector& x) {
			write_key_val<float>("curr", x.frame().currentRe);
			return *this;
		}
		uLCD28PT& operator << (const char * x) {
			write_key_val<const char *>("text", x);
			return *this;
		}
	};
}

#endif

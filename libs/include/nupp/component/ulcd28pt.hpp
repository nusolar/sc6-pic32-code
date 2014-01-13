#ifndef NUPP_ULCD28PT_HPP
#define NUPP_ULCD28PT_HPP 1

#include "nupp/peripheral/serial.hpp"
#include "nupp/peripheral/pinctl.hpp"
#include "nupp/peripheral/can.hpp"
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
		ALWAYSINLINE void write_key_val(const char *key, V value) {
			OStream::operator<< (TTY_RECORD) << (key) <<  (TTY_UNIT) << (value) << (TTY_RECORD) << (end);
		}

	public:
		ALWAYSINLINE uLCD28PT(UARTModule mod): Serial(mod) {}

		ALWAYSINLINE uLCD28PT& operator << (const can::frame::ws20::tx::motor_velocity& x) {
			write_key_val<float>("velo", x.frame().vehicleVelocity);
			return *this;
		}
		ALWAYSINLINE uLCD28PT& operator << (const can::frame::ws20::tx::current_vector& x) {
			write_key_val<float>("curr", x.frame().currentRe);
			return *this;
		}
		ALWAYSINLINE uLCD28PT& operator << (const char * x) {
			write_key_val<const char *>("text", x);
			return *this;
		}
	};
}

#endif

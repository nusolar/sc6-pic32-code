#ifndef NUPP_ULCD28PT_HPP
#define NUPP_ULCD28PT_HPP 1

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
	struct uLCD28PT: public Serial {
		static const char unit = '\x1F';
		static const char record = '\x1E';

		/**
		 * WARNING: USES HEAP. ACCEPTABLE CUZ ITS A PRINTF BUFFER,
		 * WOULD IDENTICALLY OVERFLOW ON STACK.
		 */
		std::stringstream s;

		ALWAYSINLINE uLCD28PT(UART_MODULE mod): Serial(mod), s() {}

		uLCD28PT& operator << (const can::frame::ws20::tx::motor_velocity& x) {
			write_key_val("velo", x.frame.s.vehicleVelocity);
			return *this;
		}
		uLCD28PT& operator << (const can::frame::ws20::tx::current_vector& x) {
			write_key_val("curr", x.frame.s.currentRe);
			return *this;
		}
		uLCD28PT& operator << (const char * x) {
			write_key_val("text", x);
			return *this;
		}

	private:
		template <class V>
		void write_key_val(const char *key, V &value) {
			s << record << key << unit << value << record;
			this->printf(s.str().c_str());
			s.str("");
		}
	};
}

#endif

#ifndef NUXX_LED_HPP
#define NUXX_LED_HPP 1

/**
 * @file
 * Little wrapper for LEDs
 */

#include "nuxx/peripheral/pinctl.hpp"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	class Led: protected DigitalOut {
		bool _led_status;

	public:
		Led(Pin led = Pin()): DigitalOut(Pin(led)), _led_status(false) {
			set_digital_out();
			off();
		}
		void off()		{Pin::set(); _led_status = false;}
		void on()		{Pin::clear(); _led_status = true;}
		void toggle()	{Pin::toggle(); _led_status = !_led_status;}

		/** Get whether LED is on or off. */
		bool status()		{return _led_status;}
		operator bool()	{return _led_status;}

		/** Turn LED [on/off] by assigning it to [true/false] respectively. */
		Led& operator= (const bool rhs) {
			if (rhs) on();
			else off();
			return *this;
		}

		void set(const bool rhs) {
			(rhs? on(): off());
		}
	};
}

#endif

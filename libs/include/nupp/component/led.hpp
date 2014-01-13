#ifndef NUPP_LED_HPP
#define NUPP_LED_HPP 1

/**
 * @file
 * Little wrapper for LEDs
 */

#include "nupp/peripheral/pinctl.hpp"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	class Led: protected DigitalOut {
		bool _led_status;

	public:
		INLINE Led(Pin led = Pin()): DigitalOut(Pin(led)), _led_status(false) {
			set_digital_out();
			off();
		}
		INLINE void off()		{Pin::set(); _led_status = false;}
		INLINE void on()		{Pin::clear(); _led_status = true;}
		INLINE void toggle()	{Pin::toggle(); _led_status = !_led_status;}

		/** Get whether LED is on or off. */
		INLINE bool status()		{return _led_status;}
		INLINE operator bool()	{return _led_status;}

		/** Turn LED [on/off] by assigning it to [true/false] respectively. */
		INLINE Led& operator= (const bool rhs) {
			if (rhs) on();
			else off();
			return *this;
		}

		INLINE void set(const bool rhs) {
			(rhs? on(): off());
		}
	};
}

#endif

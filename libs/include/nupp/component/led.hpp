#ifndef NUPP_LED_HPP
#define NUPP_LED_HPP 1

/**
 * @file
 * Little wrapper for LEDs
 */

#include "nupp/pinctl.hpp"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	class Led: protected DigitalOut {
		bool _led_status;

	public:
		ALWAYSINLINE Led(Pin led = Pin()): DigitalOut(Pin(led)), _led_status(false) {
			set_digital_out();
			off();
		}
		ALWAYSINLINE void off()		{Pin::set(); _led_status = false;}
		ALWAYSINLINE void on()		{Pin::clear(); _led_status = true;}
		ALWAYSINLINE void toggle()	{Pin::toggle(); _led_status = !_led_status;}

		/** Get whether LED is on or off. */
		ALWAYSINLINE bool status()		{return _led_status;}
		ALWAYSINLINE operator bool()	{return _led_status;}

		/** Turn LED [on/off] by assigning it to [true/false] respectively. */
		ALWAYSINLINE Led& operator= (const bool rhs) {
			if (rhs) on();
			else off();
			return *this;
		}

		ALWAYSINLINE void set(const bool rhs) {
			(rhs? on(): off());
		}
	};
}

#endif

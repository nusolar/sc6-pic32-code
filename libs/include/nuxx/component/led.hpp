#ifndef NUXX_LED_HPP
#define NUXX_LED_HPP 1

/**
 * @file
 * Little wrapper for LEDs
 */

#include "nuxx/peripheral/pin.hpp"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	class Led {
		DigitalOut base;
		bool _led_status;

	public:
		Led(PlatformPin led = PlatformPin()): base(PlatformPin(led)), _led_status(false)
		{
		}

		void setup()
		{
			this->base.setup();
			this->off();
		}

		void off()		{this->base.high(); _led_status = false;}
		void on()		{this->base.low(); _led_status = true;}
		void toggle()	{this->base.toggle(); _led_status = !_led_status;}

		/** Get whether LED is on or off. */
		bool status()	{return _led_status;}

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

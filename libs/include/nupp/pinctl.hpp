#ifndef NUPP_PINCTL_HPP
#define NUPP_PINCTL_HPP 1

/**
 * @file
 * Simple pin wrapper
 */

#include "nu/compiler.h"
#include "nupp/platform/pin.hpp"
#include <cstdint>

namespace nu {
	/**
	 * The universal interface for a Microcontroller Pin. It inherits from a
	 * platform-specific Pin class, which does the heavy labor.
     */
	struct AbstractPin: protected Pin {
		ALWAYSINLINE AbstractPin(Pin p = Pin()): Pin(p) {}
		NOINLINE virtual ~AbstractPin() {}

	protected:
		ALWAYSINLINE void set_digital_out()	{Pin::set_digital_out();}
		ALWAYSINLINE void set_digital_in()	{Pin::set_digital_in();}
		ALWAYSINLINE void set_analog_out()	{Pin::set_analog_out();}
		ALWAYSINLINE void set_analog_in()	{Pin::set_analog_in();}

		virtual INLINE void set()		{Pin::set();}
		virtual INLINE void clear()		{Pin::clear();}
		virtual INLINE void toggle()	{Pin::toggle();}

		/** A subclass may call EITHER read() OR read_analog(). */
		virtual INLINE reg_t read_digital()	{return Pin::read_digital();} // returns 0 or non-0
		virtual INLINE reg_t read_analog()	{return Pin::read_analog();}
	};


	struct DigitalIn: protected AbstractPin {
		ALWAYSINLINE DigitalIn(Pin p = Pin()): AbstractPin(p) {set_digital_in();}
		ALWAYSINLINE reg_t read() {return Pin::read_digital();} // returns 0 or any non-0
	};

	/** An Analog input Pin, wrapped with an ADC. */
	struct AnalogIn: protected AbstractPin {
		ALWAYSINLINE AnalogIn(Pin p = Pin()): AbstractPin(p) {set_analog_in();}
		ALWAYSINLINE reg_t read()	{return Pin::read_analog();}
	};


	class DigitalOut: protected AbstractPin {
		bool _status;

	public:
		ALWAYSINLINE DigitalOut(Pin p = Pin(), bool init = false): AbstractPin(p), _status(init) {
			set_digital_out();
			if (init) high();
			else low();
		}

		ALWAYSINLINE void high()	{set(); _status = true;}
		ALWAYSINLINE void low()		{clear(); _status = false;}
		ALWAYSINLINE void toggle()	{toggle(); _status = !_status;}
		ALWAYSINLINE bool status()	{return _status;}

		ALWAYSINLINE DigitalOut &operator= (const bool rhs) {
			if (rhs) set();
			else clear();
			return *this;
		}
	};
}

#endif

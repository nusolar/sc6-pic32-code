#ifndef NUXX_PINCTL_HPP
#define NUXX_PINCTL_HPP 1

/**
 * @file
 * Simple pin wrapper. To specify a pin, use PIN(LETTER, NUMBER)
 */

#include "nu/compiler.h"
#include "nuxx/platform/pin.hpp"
#include <cstdint>

namespace nu {
	/**
	 * The common interface for a Microcontroller Pin. It inherits from a
	 * platform-specific Pin class, which does the PLIB work.
	 */
	struct AbstractPin: protected Pin {
		ALWAYSINLINE AbstractPin(Pin p = Pin()): Pin(p) {}
		NOINLINE virtual ~AbstractPin() {}

	protected:
		ALWAYSINLINE void set_digital_out()	{Pin::set_digital_out();}
		ALWAYSINLINE void set_digital_in()	{Pin::set_digital_in();}
		ALWAYSINLINE void set_analog_out()	{Pin::set_analog_out();}
		ALWAYSINLINE void set_analog_in()	{Pin::set_analog_in();}

		void set()		{Pin::set();}
		void clear()		{Pin::clear();}
		void toggle()	{Pin::toggle();}

		/** A subclass may call EITHER read_digital() OR read_analog(). */
		reg_t read_digital()	{return Pin::read_digital();} // returns 0 or non-0
		reg_t read_analog()	{return Pin::read_analog();}
	};


	struct DigitalIn: protected AbstractPin {
		ALWAYSINLINE DigitalIn(Pin p = Pin()): AbstractPin(p) {set_digital_in();}
		ALWAYSINLINE reg_t read() {return Pin::read_digital();} // returns 0 or some non-0
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

		ALWAYSINLINE void high()	{AbstractPin::set(); _status = true;}
		ALWAYSINLINE void low()		{clear(); _status = false;}
		ALWAYSINLINE void toggle()	{toggle(); _status = !_status;}
		ALWAYSINLINE bool status()	{return _status;}
		ALWAYSINLINE void set(bool rhs) {
			if (rhs) AbstractPin::set();
			else clear();
		}
	};
}

#endif

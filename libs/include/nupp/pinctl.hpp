#ifndef NUPP_PINCTL_HPP
#define NUPP_PINCTL_HPP 1

/**
 * @file
 * Simple pin wrapper
 */

#include <string>
#include "nu/compiler.h"
#include <cstdint>
#include <plib.h>
#include <alloca.h>

namespace nu {
	/**
	 * Encapsulate a PIC32 I/O pin. They are addressed by a (letter, number)
	 * combination, represented by an (IoPortId, BIT_X) type combination.
	 */
	struct Pin {
		IoPortId ltr;
		uint32_t num;
		const char *name;


		/**
		 * Setup Pin's (letter, number) combination, represented by an
		 * (IoPortId, BIT_X) type combination.
		 */
		ALWAYSINLINE Pin(IoPortId _ltr = IOPORT_D, uint32_t _num = BIT_0, const char *_name = ""):
			ltr(_ltr), num(_num), name{_name} {}
		NOINLINE virtual ~Pin() {}
		ALWAYSINLINE Pin(const Pin& p): ltr(p.ltr), num(p.num), name(p.name) {}
		ALWAYSINLINE Pin& operator =(const Pin& p) {ltr=p.ltr; num=p.num; name=p.name; return *this;}

		/**
		 * Call one of these four setters in your Setup.
		 */
		void ALWAYSINLINE set_digital_out()	{PORTSetPinsDigitalOut(ltr, num);}
		void ALWAYSINLINE set_digital_in()	{PORTSetPinsDigitalIn(ltr, num);}
		void ALWAYSINLINE set_analog_out()	{PORTSetPinsAnalogOut(ltr, num);}
		void ALWAYSINLINE set_analog_in()	{PORTSetPinsAnalogIn(ltr, num);}


		uint32_t ALWAYSINLINE read() {return PORTReadBits(ltr, num);} // returns 0 or not-0
		void ALWAYSINLINE set()		{PORTSetBits(ltr, num);}
		void ALWAYSINLINE clear()	{PORTClearBits(ltr, num);}
		void ALWAYSINLINE toggle()	{PORTToggleBits(ltr, num);}


		void ALWAYSINLINE high()	{set();}
		void ALWAYSINLINE low()		{clear();}


		/**
		 * Easily [raise/lower] Pin by setting it to [true/false] respectively.
		 */
		ALWAYSINLINE Pin &operator= (bool rhs) {
			if (rhs) set();
			else clear();
			return *this;
		}
	};
}

#endif

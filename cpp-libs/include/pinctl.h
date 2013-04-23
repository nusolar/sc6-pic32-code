#ifndef NUSOLAR_CPP_PINCTL_H
#define NUSOLAR_CPP_PINCTL_H 1

#include "compiler.h"
#include <cstdint>
#include <plib.h>

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
		INLINE Pin(IoPortId _ltr = IOPORT_D, uint32_t _num = 0, const char *_name = ""):
			ltr(_ltr), num(_num), name(_name) {}

		/**
		 * Call one of these four setters in your Setup.
		 */
		void ALWAYSINLINE set_digital_out()	{PORTSetPinsDigitalOut(ltr, num);}
		void ALWAYSINLINE set_digital_in()	{PORTSetPinsDigitalIn(ltr, num);}
		void ALWAYSINLINE set_analog_out()	{PORTSetPinsAnalogOut(ltr, num);}
		void ALWAYSINLINE set_analog_in()	{PORTSetPinsAnalogIn(ltr, num);}

		
		unsigned int ALWAYSINLINE read() {return PORTReadBits(ltr, num);}
		void ALWAYSINLINE set()		{PORTSetBits(ltr, num);}
		void ALWAYSINLINE clear()	{PORTClearBits(ltr, num);}
		void ALWAYSINLINE toggle()	{PORTToggleBits(ltr, num);}
		
		
		void ALWAYSINLINE high()	{set();}
		void ALWAYSINLINE low()		{clear();}
		
		
		/**
		 * Easily [raise/lower] Pin by setting it to [true/false] respectively.
		 */
		ALWAYSINLINE Pin &operator= (bool rhs) {
			if (rhs)
				set();
			else
				clear();
			return *this;
		}
	};
}

#endif

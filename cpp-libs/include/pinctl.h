
//
//  Pinctl.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/12/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__pinctl__
#define __nusolar_lib__pinctl__

#include "compiler.h"
#include <cstdint>

#include <plib.h>

namespace nu {
	struct Pin {
		IoPortId ltr;
		uint32_t num;
		const char *name;

		
		/**
		 * Encapsulate a PIC32 I/O pin. They are addressed by a (letter, number)
		 * combination, represented by an (IoPortId, BIT_X) type combination.
		 */
		INLINE Pin(IoPortId ltr = IOPORT_D, uint32_t num = 0, const char *name = ""):
			ltr(ltr), num(num), name(name) {}

		
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
		bool ALWAYSINLINE operator= (bool setting) {
			if (setting) set();
			else clear();
			return setting;
		}
	};
}

#endif /* defined(__nusolar_lib__Pinctl__) */

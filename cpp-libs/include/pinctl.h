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
#include <stdint.h>
#include <peripheral/ports.h>

namespace nu {
	struct Pin {
		IoPortId ltr;
		uint32_t num;
		const char *name;
		
		Pin(IoPortId ltr, uint32_t num, const char *name = ""): ltr(ltr), num(num), name(name) {}
		
		void ALWAYSINLINE set_digital_out() {
			PORTSetPinsDigitalOut(ltr, num);
		}
		void ALWAYSINLINE set_digital_in() {
			PORTSetPinsDigitalIn(ltr, num);
		}
		void ALWAYSINLINE set_analog_out() {
			PORTSetPinsAnalogOut(ltr, num);
		}
		void ALWAYSINLINE set_analog_in() {
			PORTSetPinsAnalogIn(ltr, num);
		}
		
		unsigned int ALWAYSINLINE read() {
			return PORTReadBits(ltr, num);
		}
		void ALWAYSINLINE set() {
			PORTSetBits(ltr, num);
		}
		void ALWAYSINLINE clear() {
			PORTClearBits(ltr, num);
		}
		void ALWAYSINLINE toggle() {
			PORTToggleBits(ltr, num);
		}
	};
}

#endif /* defined(__nusolar_lib__Pinctl__) */

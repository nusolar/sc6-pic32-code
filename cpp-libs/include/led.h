//
//  led.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/13/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__led__
#define __nusolar_lib__led__

#include "pinctl.h"

namespace nu {
	/**
	 * Wrap Led setup, clarify Pin function names.
	 */
	struct Led: protected Pin {
		ALWAYSINLINE Led(IoPortId ltr = IOPORT_D, uint32_t num = 0, const char *name = ""):
			Pin(ltr, num, name) {}
		ALWAYSINLINE Led(Pin led): Pin(led) {}
		
		void ALWAYSINLINE off() {set();}
		void ALWAYSINLINE on() {clear();}
		void ALWAYSINLINE toggle() {Pin::toggle();}
		unsigned int ALWAYSINLINE status() {return read();}
		
		void ALWAYSINLINE setup() {
			set_digital_out();
			off();
		}
	};
}

#endif /* defined(__nusolar_lib__led__) */

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
	struct Led: protected Pin {
		Led(IoPortId ltr, uint32_t num, const char *name = ""): Pin(ltr, num, name) {}
		
		void ALWAYSINLINE off() {set();}
		void ALWAYSINLINE on() {clear();}
		void ALWAYSINLINE toggle() {Pin::toggle();}
		unsigned int ALWAYSINLINE status() {return read();}
		
		void INLINE setup() {
			set_digital_out();
			off();
		}
	};
}

#endif /* defined(__nusolar_lib__led__) */

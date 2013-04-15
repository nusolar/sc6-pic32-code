//
//  button.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/12/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__button__
#define __nusolar_lib__button__

#include "pinctl.h"

namespace nu {
	struct Button: protected Pin {
		uint8_t debounce;
		uint8_t debounce_max;
		uint8_t thresh;
		
		INLINE
		Button(IoPortId ltr, uint32_t num, uint8_t _debounce_max, uint8_t _thresh, const char *name = ""): Pin(ltr, num, name), debounce_max(_debounce_max), thresh(_thresh) {}
		
		bool ALWAYSINLINE pressed() {
			return debounce >= thresh;
		}
		void INLINE update() {
			debounce += read()? 1: -1;
			if (debounce < 0) debounce = 0;
			if (debounce > debounce_max) debounce = debounce_max;
		}
	};
}

#endif /* defined(__nusolar_lib__button__) */

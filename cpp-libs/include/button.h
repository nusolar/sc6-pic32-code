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
	/**
	 * Abstract Button updates and detecting pressed buttons. 
	 */
	struct Button: protected Pin {
		int8_t debounce;
		uint8_t debounce_max;
		uint8_t thresh;
		
		
		ALWAYSINLINE Button(Pin btn = Pin(), uint8_t _debounce_max = 10, uint8_t _thresh = 5):
			Pin(btn), debounce(0), debounce_max(_debounce_max), thresh(_thresh) {}
		
		bool ALWAYSINLINE pressed() {
			return debounce >= thresh;
		}
		void ALWAYSINLINE update() {
			debounce += (read()? 1: -1);
			if (debounce < 0) debounce = 0;
			if (debounce > debounce_max) debounce = debounce_max;
		}
	};
}

#endif /* defined(__nusolar_lib__button__) */

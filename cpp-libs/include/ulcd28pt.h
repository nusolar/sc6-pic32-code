//
//  ulcd28pt.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/20/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__ulcd28pt__
#define __nusolar_li

#include <cstdint>
#include <cstddef>

#include "serial.h"
#include "pinctl.h"

namespace nu {
	struct uLCD28PT: public Serial {
		
		uLCD28PT(UART_MODULE mod): Serial(mod) {}
		
	};
}

#endif /* defined(__nusolar_lib__ulcd28pt__) */

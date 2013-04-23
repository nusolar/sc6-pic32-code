//
//  param.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/20/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__param__
#define __nusolar_lib__param__

#include <cstdint>

namespace nu {
	/**
	 * Operating hardware-specific parameters.
	 */
	struct Param {
		static uint64_t nu_hz; // FUCK MPLAB
		
		#define NU_DEFAULT_HZ (80000000UL)
		#define NU_PBUS_FREQ_HZ (Param::nu_hz/((uint32_t)(1 << OSCCONbits.PBDIV)))
		#define HZ (80000000UL)
	};
}

#endif /* defined(__nusolar_lib__param__) */

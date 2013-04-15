//
//  can.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__can__
#define __nusolar_lib__can__

#include <stdint.h>

#include <peripheral/CAN.h>

namespace nu {
	struct CAN {
		enum id_type {
			STANDARD_ID,
			EXTENDED_ID
		};
		
		CAN_CHANNEL chn;
		id_type id_type;
		uint16_t std_id;
		uint16_t ext_id;
		
		
	};
}

#endif /* defined(__nusolar_lib__can__) */

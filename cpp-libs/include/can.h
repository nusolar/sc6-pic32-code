//
//  can.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/14/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__can__
#define __nusolar_lib__can__

#include "compiler.h"
#include <stdint.h>
#include <GenericTypeDefs.h>

#include <peripheral/CAN.h>

namespace nu {
	struct CAN {
		enum id_type {
			STANDARD_ID,
			EXTENDED_ID
		};
		
		CAN_MODULE module;
		CAN_CHANNEL chn;
		id_type type;
		uint16_t std_id;
		uint16_t ext_id;
		char buf[32*32*CAN_TX_RX_MESSAGE_SIZE_BYTES];
		
		INLINE
		CAN(CAN_MODULE _mod, CAN_CHANNEL _chn, id_type _type, 
                uint16_t _std, uint16_t _ext): module(_mod), chn(_chn),
                type(_type), std_id(_std), ext_id(_ext) {}

                static void ALWAYSINLINE vend(CAN &can, CAN_MODULE _mod) {
                    can = CAN(_mod, CAN_CHANNEL0, STANDARD_ID, 0, 0);
                }
	};
}

#endif /* defined(__nusolar_lib__can__) */

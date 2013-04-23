//
//  ad7685.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/20/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__ad7685__
#define __nusolar_lib__ad7685__

#include <cstdint>
#include "compiler.h"
#include "errorcodes.h"

#include "spi.h"
#include "pinctl.h"

namespace nu {
	/**
	 *  The Analog-to-Digital Converter used on the BMS current sensor
	 */
	struct AD7685: protected SPI {
		enum options {
			NONE = 0,
			THREE_WIRE = 0,
			FOUR_WIRE = 1<<0,
			CHAIN_MODE = 1<<1 | 1<<0,
			NO_BUSY_INDICATOR = 0,
			BUSY_INDICATOR = 1<<2
		};
		
		Pin convert;
		uint32_t num_devices;
		options opts;
		
		ALWAYSINLINE AD7685(SPI spi, Pin _convert, uint32_t _num, options _opts):
			SPI(spi), convert(_convert), num_devices(_num), opts(_opts) {}
		
		/**
		 * Setup SPI and conversion pin.
		 */
		int32_t ALWAYSINLINE setup() {
			if ((FOUR_WIRE & opt && NO_BUSY_INDICATOR & opt) ||
				(CHAIN_MODE & opt && BUSY_INDICATOR & opt))
				return -EINVAL; // TODO: C++ exceptions
			convert.set_digital_out();
			SPI::setup(100000, (SpiOpenFlags)(SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON));
			return 0;
		}
		
		/** 
		 * Gets the actual voltage reading(s) (not raw data).
		 */
		void convert_read_uv(uint32_t *dst);
		
	private:
		void read_uv(uint32_t *dst);
	};
}

#endif /* defined(__nusolar_lib__ad7685__) */

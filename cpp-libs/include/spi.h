//
//  spi.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/12/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__spi__
#define __nusolar_lib__spi__

#include "param.h"
#include <stddef.h> // size_t

#include "pinctl.h"
#include <string.h>
#include <peripheral/spi.h>

namespace nu {
	struct SPI: protected Pin { // chip select pin
		enum options {
			SPI_DEFAULT = 0,
		};
		
		enum tx_options {
			SPI_TX_WAIT_START = 1<<0,
			SPI_TX_WAIT_END = 1<<1
		};
		
		SpiChannel chn;
		options opt;
		
		SPI(IoPortId ltr, uint32_t num, SpiChannel _chn, options _opt, const char *name = ""): Pin(ltr, num, name), chn(_chn), opt(_opt) {}
		
		void ALWAYSINLINE setup(uint32_t bitrate, SpiOpenFlags oflags) {
			SpiChnOpen(chn, oflags, (uint32_t) NU_PBUS_FREQ_HZ/bitrate);
		}
		void setup_pin(uint32_t bitrate, SpiOpenFlags oflags) {
			set_digital_out();
			high();
			setup(bitrate, oflags);
		}
		
		void ALWAYSINLINE high() {set();}
		void ALWAYSINLINE low() {clear();}
		
		void ALWAYSINLINE rx(void *dst, size_t n);
		void ALWAYSINLINE tx(const void *src, size_t n, tx_options _opt);
		void ALWAYSINLINE puts(const char *str, tx_options _opt) {
			tx(str, strlen(str), _opt);
		}
		
	private:
		static uint32_t get_bitrate(SpiChannel chn);
		void wait_busy();
	};
}

#endif /* defined(__nusolar_lib__spi__) */

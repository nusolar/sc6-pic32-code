#ifndef NUPP_AD7685_HPP
#define NUPP_AD7685_HPP 1

#include <cstdint>
#include "nu/compiler.h"
#include "nupp/errorcodes.hpp"

#include <alloca.h>
#include <sys/endian.h>
#include "nupp/timer.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/spi.hpp"

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

		DigitalOut convert;
		uint32_t num_devices;
		options opts;

		ALWAYSINLINE AD7685(Pin _cs, SpiChannel _chn, Pin _convert, uint32_t _num, options _opts):
			SPI(_cs, _chn, 100000, (SpiOpenFlags)(SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON)),
			convert(_convert), num_devices(_num), opts(_opts) {
			if ((FOUR_WIRE & opt && NO_BUSY_INDICATOR & opt) ||
				(CHAIN_MODE & opt && BUSY_INDICATOR & opt))
				return /*-EINVAL*/; // TODO: C++ exceptions
		}

		/**
		 * Gets the actual voltage reading(s) (not raw data).
		 */
		ALWAYSINLINE void convert_read_uv(uint32_t *dst){
			if (FOUR_WIRE & opt && BUSY_INDICATOR & opt)
				cs.high();

			// start conversion
			convert.high();
			timer::delay_ns(100);  // .1 us

			if (BUSY_INDICATOR & opt) {
				if (THREE_WIRE & opt)
					convert.low();
				else if (FOUR_WIRE & opt)
					cs.high();
			}

			timer::delay_ns(2300); // 2.3 us

			if (THREE_WIRE & opt && NO_BUSY_INDICATOR & opt)
				cs.low();

			// read in the actual voltage reading(s) over SPI
			read_uv(dst);

			cs.low();
			timer::delay_us(5);
		}

	private:
		ALWAYSINLINE void read_uv(uint32_t *dst){
			uint16_t *buffer = (uint16_t *)alloca(sizeof(*buffer) * num_devices);

			rx(buffer, sizeof(*buffer)*num_devices);
			for (unsigned int ui=0; ui < num_devices; ui++) {
				// swap byte order
				// buf[ui] = bswap_u16(buf[ui]);
				buffer[ui] = betoh16(buffer[ui]);
				// then compute the actual voltage (microvolts)
				dst[ui] = (uint32_t)((5000000 * (uint64_t)buffer[ui])>>16);
			}
		}
	};
}

#endif

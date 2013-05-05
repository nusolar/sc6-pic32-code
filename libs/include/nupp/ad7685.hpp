#ifndef NUPP_AD7685_HPP
#define NUPP_AD7685_HPP 1

#include <cstdint>
#include "nu/compiler.h"
#include "nupp/errorcodes.hpp"

#include "nupp/spi.hpp"
#include "nupp/pinctl.hpp"

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
		void convert_read_uv(uint32_t *dst);

	private:
		void read_uv(uint32_t *dst);
	};
}

#endif

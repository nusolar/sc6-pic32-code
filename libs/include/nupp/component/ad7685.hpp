#ifndef NUPP_AD7685_HPP
#define NUPP_AD7685_HPP 1

#include "nu/compiler.h"
#include "nupp/array.hpp"
#include "nupp/spi.hpp"
#include "nupp/timer.hpp"
#include "nupp/pinctl.hpp"
#include "nupp/errorcodes.hpp"
#include <cstdint>
#include <sys/endian.h>


namespace nu {
	/**
	 * The Analog-to-Digital Converter used with the BMS current sensors.
	 * AD7685 takes num_devices as a template parameter. Cached values are
	 * accessible via array subscripting. Call convert_read_uv to update these.
	 */
	template <uint32_t num_devices>
	struct AD7685: protected SPI {
		enum options {
			NONE = 0,
			THREE_WIRE = 0,
			FOUR_WIRE = 1<<0,
			CHAIN_MODE = 1<<1 | 1<<0,
			NO_BUSY_INDICATOR = 0,
			BUSY_INDICATOR = 1<<2,

			CHAIN_MODE_NO_BUSY = CHAIN_MODE|NO_BUSY_INDICATOR
		};

		DigitalOut convert; // ERROR same as CS pin?
		options opts;
		
		Array<uint32_t, num_devices> values;
		/** @warning NO BOUNDS CHECKING */
		ALWAYSINLINE uint32_t operator[] (size_t index) const {return values[index];}
		ALWAYSINLINE uint32_t count() {return num_devices;}

		/**
		 * Construct an AD7685 interface over SPI, with a Convert pin.
         * @param _cs The SPI Chip Select pin.
         * @param _chn
         * @param _convert Used to control AD7685. Identical to CS in some configurations.
         * @param _opts
         */
		ALWAYSINLINE AD7685(Pin _cs, uint8_t _channel, Pin _convert, options _opts):
			SPI(_cs, Spi(_channel, 100000, SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON)),
			convert(_convert), opts(_opts), values(0U)
		{
			if ((FOUR_WIRE & opt && NO_BUSY_INDICATOR & opt) ||
				(CHAIN_MODE & opt && BUSY_INDICATOR & opt))
				return /*-EINVAL*/; // TODO: C++ exceptions
		}
		
		/**
		 * Gets the actual voltage reading(s) (not raw data).
		 */
		ALWAYSINLINE void convert_read_uv(){
			if (FOUR_WIRE & opt && BUSY_INDICATOR & opt)
				cs.high();

			// start conversion
			convert.high();
			timer::delay_ns<100>();  // .1 us

			if (BUSY_INDICATOR & opt) {
				if (THREE_WIRE & opt)
					convert.low();
				else if (FOUR_WIRE & opt)
					cs.high();
			}

			// 2.3 us
			timer::delay_us<2>();
			timer::delay_ns<300>();

			if (THREE_WIRE & opt && NO_BUSY_INDICATOR & opt)
				cs.low();

			// read in the actual voltage reading(s) over SPI
			read_uv();

			cs.low();
			timer::delay_us<5>();
		}

	private:
		/**
		 * Read SPI data from AD7685 into buffer.
         */
		ALWAYSINLINE void read_uv(){
			Array<uint16_t, num_devices> buffer;

			rx(buffer, buffer.size());
			for (unsigned int ui=0; ui < num_devices; ui++) {
				// swap byte order
				// buf[ui] = bswap_u16(buf[ui]);
				buffer[ui] = betoh16(buffer[ui]);
				// then compute the actual voltage (microvolts)
				values[ui] = (uint32_t)((5000000 * (uint64_t)buffer[ui])>>16);
			}
		}
	};
}

#endif

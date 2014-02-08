#ifndef NUPP_AD7685_HPP
#define NUPP_AD7685_HPP 1

#include "nupp/peripheral/spi.hpp"
#include "nupp/peripheral/pinctl.hpp"
#include "nupp/array.hpp"
#include "nupp/timer.hpp"
#include "nupp/errorcodes.hpp"
#include "nu/compiler.h"
#include <cstdint>
#include <sys/endian.h>


namespace nu {
	/**
	 * The Analog-to-Digital Converter used with the BMS current sensors.
	 * AD7685's num_devices determines how many UInt16's are read.
	 *
	 * In THREE_WIRE & CHAIN_MODE, no chip-select pin is used.
	 *
	 * The AD7685 returns its voltages in multiples of 76.294 microvolts,
	 * as UInt16's.
	 * HOWEVER, this class returns readings in microvolts, as UInt32's.
	 *
	 * Call convert_read_uv() to synchronously poll & return readings.
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

		/**
		 * Construct an AD7685 interface over SPI, with a Convert pin.
         * @param _cs The SPI Chip Select pin.
         * @param _chn
         * @param _convert Used to control AD7685. Identical to CS in some configurations.
         * @param _opts
         */
		AD7685(Pin _cs, uint8_t _channel, Pin _convert, options _opts):
			SPI(_cs, Spi(_channel, 100000, SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON)),
			convert(_convert), opts(_opts)
		{
			if ((FOUR_WIRE & opt && NO_BUSY_INDICATOR & opt) ||
				(CHAIN_MODE & opt && BUSY_INDICATOR & opt))
				return /*-EINVAL*/; // TODO: C errors
		}

		/**
		 * Gets the actual voltage reading(s) (not raw data).
		 */
		void convert_read_uv(Array<uint32_t, num_devices> &values){
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

			// 2.3 us
			timer::delay_us(2);
			timer::delay_ns(300);

			if (THREE_WIRE & opt && NO_BUSY_INDICATOR & opt)
				cs.low();

			// read in the actual voltage reading(s) over SPI
			Array<uint16_t, num_devices> buffer;
			read_raw(buffer);

			// populate values
			for (unsigned iCurrent=0; iCurrent<num_devices; ++iCurrent)
			{
				values[iCurrent] = convert_uv(buffer[iCurrent]);
			}

			cs.low();
			timer::delay_us(5);
		}

		/**
		 * Compute compute the actual reading in microvolts
		 * Max value is 0xffff --> 4999923 uV
		 */
		uint32_t convert_uv(uint16_t val){
			return (uint32_t)((5000000 * (uint64_t)val)>>16);
		}

	private:
		/**
		 * Read SPI data from AD7685 into buffer, and byteswap.
         */
		void read_raw(Array<uint16_t, num_devices> &buffer){
			rx(buffer, buffer.size());
			for (unsigned int iCurrent=0; iCurrent < num_devices; ++iCurrent) {
				// swap byte order
				// buf[ui] = bswap_u16(buf[ui]);
				buffer[iCurrent] = betoh16(buffer[iCurrent]);
			}
		}
	};
}

#endif

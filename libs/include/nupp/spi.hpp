#ifndef NUPP_SPI_HPP
#define NUPP_SPI_HPP 1

#include "nupp/param.hpp"
#include <cstddef> // size_t

#include "nupp/pinctl.hpp"
#include <cstring> // strlen
#include <plib.h>

namespace nu {
	/**
	 * Encapsulate SPI reading/writing.
	 */
	struct SPI { // chip select pin
		enum UNUSED options {
			DEFAULT = 0,
		};
		enum tx_options {
			TX_WAIT_START = 1<<0,
			TX_WAIT_END = 1<<1
		};

		/**
		 * Chip Select pin. Used to tell an SPI device to listen. Whether it's
		 * high or low depends on the SPI Thing in question.
		 */
		DigitalOut cs;
		SpiChannel chn;
		tx_options opt;

		ALWAYSINLINE SPI(Pin _cs, SpiChannel _chn, uint32_t bitrate, SpiOpenFlags oflags,
						 tx_options _opt = (tx_options)(TX_WAIT_START|TX_WAIT_END)):
			cs(_cs), chn(_chn), opt(_opt) {
			cs.high();
			SpiChnOpen(chn, oflags, (uint32_t) param::pbus_hz()/bitrate);
		}
		NOINLINE virtual ~SPI() {};
			
			
		void rx(void *dst, size_t n);
		void tx(const void *src, size_t n);
		virtual void ALWAYSINLINE puts(const char *str) {
			tx(str, strlen(str));
		}

	private:
		static uint32_t get_bitrate(SpiChannel chn);
		void wait_busy();
	};
}

#endif

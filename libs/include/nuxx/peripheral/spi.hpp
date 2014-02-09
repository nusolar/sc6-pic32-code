#ifndef NUXX_SPI_HPP
#define NUXX_SPI_HPP 1

#include "nuxx/platform/spi.hpp"
#include "nuxx/peripheral/pinctl.hpp"
#include "nuxx/stream.hpp"
#include "nuxx/param.hpp"
#include <cstddef> // size_t
#include <cstring> // strlen

namespace nu {
	/**
	 * Encapsulate SPI reading/writing.
	 */
	struct SPI: public OStream, protected SpiModule { // chip select pin
		enum tx_options {
			TX_WAIT_START = 1<<0,
			TX_WAIT_END = 1<<1,
			TX_DISABLE_AUTO_CS = 1<<2
		};

		/**
		 * Chip Select pin. Used to tell an SPI device to listen.
		 * Set it high or low depending on the SPI Thing in question.
		 */
		DigitalOut cs;
		tx_options opt;

		/**
		 * Construct SPI interface, starting with Chip Select on high.
         * @param _cs
         * @param _chn
         * @param bitrate
         * @param oflags
         * @param _opt
         */
		SPI(Pin _cs, SpiModule _mod, tx_options _opt = (tx_options)(TX_WAIT_START|TX_WAIT_END)):
			OStream(), SpiModule(_mod), cs(_cs, true), opt(_opt) {}
		NOINLINE virtual ~SPI() {};

		void rx(void *dst, size_t n) {
			SpiModule::rx(dst,n);
		}

		void tx(const void *src, size_t n) {
			if (!(opt & TX_DISABLE_AUTO_CS))
				cs.low();
			if (opt & TX_WAIT_START)
				wait_busy();

			SpiModule::tx(src,n);

			if (opt & TX_WAIT_END)
				wait_busy();
			if (!(opt & TX_DISABLE_AUTO_CS))
				cs.high();
		}

		virtual void puts(const char *str)	{tx(str, strlen(str));}
	};
}

#endif

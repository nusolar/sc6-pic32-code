#ifndef NUXX_SPI_HPP
#define NUXX_SPI_HPP 1



#include "nuxx/peripheral/pin.hpp"
#include "nu/platform.h"
#include "nuxx/timer.hpp"
#include "nu/compiler.h"

#if NU_PLATFORM==NU_PLATFORM_PIC32MX /* PIC32MX-specific SPI code */
extern "C" {
#include <plib.h>
#include <../../pic32-libs/peripheral/spi/source/_spi_map_tbl.h>
}

#define SPI(N,B,FLAGS) PlatformSpi((SpiChannel)(N), (B), (SpiOpenFlags)(FLAGS))

namespace nu {
	struct PlatformSpi {
		SpiChannel chn;
		SpiOpenFlags oflags;
		uint32_t bitrate;

		PlatformSpi(SpiChannel _chn, uint32_t _bitrate, SpiOpenFlags _oflags):
			chn(_chn), oflags(_oflags), bitrate(_bitrate) {}
		virtual ~PlatformSpi() {}

		void setup()
		{
			SpiChnOpen(chn, oflags, (uint32_t) param::pbus_hz()/bitrate);
		}

		INLINE uint32_t get_bitrate() {
			uint32_t clk_div = (_SpiMapTbl[chn]->brg+1)*2;
			return (uint32_t) param::pbus_hz()/clk_div; //bitrate
		}

		void wait_busy() {
			uint32_t bit_time_ns = 1000000000/this->get_bitrate();
			while (SpiChnIsBusy(chn)) {
				Nop();
			}
			timer::delay_ns(bit_time_ns);
		}

		void tx(const void *src, size_t n) {
			uint32_t ui;
			if (_SpiMapTbl[chn]->con.MODE32) {
				const uint32_t *elems = (const uint32_t *)src;
				SpiChnWriteC(chn, elems[0]);
				for (ui = 1; ui < n; ++ui)
					SpiChnPutC(chn, elems[ui]);
			} else if (_SpiMapTbl[chn]->con.MODE16) {
				const uint16_t *elems = (const uint16_t *)src;
				SpiChnWriteC(chn, elems[0]);
				for (ui = 1; ui < n; ++ui)
					SpiChnPutC(chn, elems[ui]);
			} else {    /* 8-bit mode */
				const uint8_t *elems = (const uint8_t *)src;
				SpiChnWriteC(chn, elems[0]);
				for (ui = 1; ui < n; ++ui)
					SpiChnPutC(chn, elems[ui]);
			}
		}

		void rx(void *dst, size_t n) {
			uint32_t ui;

			while (SpiChnRxBuffCount(chn)) {
				SpiChnReadC(chn);
			}

			if (_SpiMapTbl[chn]->con.MODE32) {
				uint32_t *elems = (uint32_t *)dst;
				for (ui = 0; ui < n; ++ui) {
					SpiChnPutC(chn, 0);
					SpiChnGetRov(chn, 1);
					elems[ui] = SpiChnGetC(chn);
				}
			} else if (_SpiMapTbl[chn]->con.MODE16) {
				uint16_t *elems = (uint16_t *)dst;
				for (ui = 0; ui < n; ++ui) {
					SpiChnPutC(chn, 0);
					SpiChnGetRov(chn, 1);
					elems[ui] = (uint16_t)SpiChnGetC(chn);
				}
			} else {    /* 8-bit mode */
				uint8_t *elems = (uint8_t *)dst;
				for (ui = 0; ui < n; ++ui) {
					SpiChnPutC(chn, 0);
					SpiChnGetRov(chn, 1);
					elems[ui] = (uint8_t)SpiChnGetC(chn);
				}
			}

			for (ui=0; ui<n; ui++)
			{
				debugger("%hX", ((uint8_t*)dst)[ui]);
			}
			debugger("\n");
		}
	};
}
#endif /* PLATFORM-specific SPI code */




#include "nuxx/stream.hpp"
#include "nuxx/param.hpp"
#include <cstddef> // size_t
#include <cstring> // strlen

namespace nu {
	/**
	 * Encapsulate SPI reading/writing.
	 */
	struct Spi: public OStream { // chip select pin
		enum tx_options {
			TX_WAIT_START = 1<<0,
			TX_WAIT_END = 1<<1,
			TX_DISABLE_AUTO_CS = 1<<2
		};

		PlatformSpi base;
		/**
		 * Chip Select pin. Used to tell a SPI device to listen.
		 * Set it high or low depending on the SPI device in question.
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
		Spi(PlatformPin _cs, PlatformSpi _mod, tx_options _opt = (tx_options)(TX_WAIT_START|TX_WAIT_END)):
			OStream(), base(_mod), cs(_cs, true), opt(_opt) {}
		
		void setup()
		{
			this->base.setup();
		}

		void rx(void *dst, size_t n)
		{
			this->base.rx(dst,n);
		}

		void tx(const void *src, size_t n)
		{
			if (!(this->opt & TX_DISABLE_AUTO_CS))
				this->cs.low();
			if (this->opt & TX_WAIT_START)
				this->base.wait_busy();

			this->base.tx(src, n);

			if (this->opt & TX_WAIT_END)
				this->base.wait_busy();
			if (!(this->opt & TX_DISABLE_AUTO_CS))
				this->cs.high();
		}

		void puts(const char *str)	{this->tx(str, strlen(str));}
	};
}

#endif

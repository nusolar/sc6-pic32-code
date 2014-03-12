/*
 * File:   spi.hpp
 * Author: alex
 *
 * Created on July 7, 2013, 10:52 AM
 */

#ifndef NU_PLATFORM_SPI_HPP
#define	NU_PLATFORM_SPI_HPP 1

#include "nuxx/peripheral/pin.hpp"
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

		PlatformSpi(SpiChannel _chn, SpiOpenFlags _oflags, uint32_t _bitrate):
			chn(_chn), oflags(_oflags), bitrate(_bitrate) {}
		virtual ~PlatformSpi() {}

		void setup()
		{
			SpiChnOpen(chn, oflags, (uint32_t) param::pbus_hz()/bitrate);
		}

		INLINE uint32_t get_bitrate(SpiChannel chn) {
			uint32_t clk_div = (_SpiMapTbl[chn]->brg+1)*2;
			return (uint32_t) param::pbus_hz()/clk_div; //bitrate
		}

		void wait_busy(SpiChannel chn) {
			uint32_t bit_time_ns = 1000000000/this->get_bitrate(chn);
			while (SpiChnIsBusy(chn)) {
				Nop();
			}
			timer::delay_ns(bit_time_ns);
		}

		void tx(SpiChannel chn, const void *src, size_t n) {
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

		void rx(SpiChannel chn, void *dst, size_t n) {
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
		}
	};
}

#endif /* PLATFORM-specific SPI code */

#endif	/* NU_PLATFORM_SPI_HPP */


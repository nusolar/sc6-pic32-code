/* 
 * File:   spi.hpp
 * Author: alex
 *
 * Created on July 7, 2013, 10:52 AM
 */

#ifndef NU_PLATFORM_SPI_HPP
#define	NU_PLATFORM_SPI_HPP

#include "nupp/timer.hpp"
#include "nupp/pinctl.hpp"
extern "C" {
#include <plib.h>
#include <../../pic32-libs/peripheral/spi/source/_spi_map_tbl.h>
}

#define Spi(N,B,FLAGS) SpiModule((SpiChannel)(N), (B), (SpiOpenFlags)(FLAGS))

namespace nu {
	struct SpiModule {
		SpiChannel chn;

		ALWAYSINLINE SpiModule(SpiChannel _chn, uint32_t bitrate, SpiOpenFlags oflags):
			chn(_chn)
		{
			SpiChnOpen(chn, oflags, (uint32_t) param::pbus_hz()/bitrate);
		}
		virtual ~SpiModule() {}

		static ALWAYSINLINE uint32_t get_bitrate(SpiChannel chn) {
			uint32_t clk_div = (_SpiMapTbl[chn]->brg+1)*2;
			return (uint32_t) param::pbus_hz()/clk_div; //bitrate
		}

		ALWAYSINLINE void wait_busy() {
			uint32_t bit_time_ns = 1000000000/this->get_bitrate(chn);
			while (SpiChnIsBusy(chn)) {
				Nop();
			}
			timer::delay_ns(bit_time_ns);
		}

		ALWAYSINLINE void tx(const void *src, size_t n) {
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

		ALWAYSINLINE void rx(void *dst, size_t n) {
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

#endif	/* NU_PLATFORM_SPI_HPP */


//
//  spi.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/12/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "spi.h"
#include "timer.h"
#include <../../pic32-libs/peripheral/spi/source/_spi_map_tbl.h>

using namespace nu;

uint32_t ALWAYSINLINE SPI::get_bitrate(SpiChannel chn) {
	uint32_t clk_div = (_SpiMapTbl[chn]->brg+1)*2;
	return (uint32_t) NU_PBUS_FREQ_HZ/clk_div; //bitrate
}

void ALWAYSINLINE SPI::wait_busy() {
	uint32_t bit_tims_ns = 1000000000/this->get_bitrate(chn);
	while (SpiChnIsBusy(chn)) {
		Nop();
	}
	delay_ns(bit_tims_ns);
}

void SPI::tx(const void *src, size_t n) {
	uint32_t ui;
	
    if (opt & TX_WAIT_START)
        wait_busy();
	
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
	
    if (opt & TX_WAIT_END)
        wait_busy();
}

void SPI::rx(void *dst, size_t n) {
	uint32_t ui;
	
	while (SpiChnRxBuffCount(chn)) {
		SpiChnReadC(chn);
	}
	
	if (_SpiMapTbl[chn]->con.MODE32) {
		uint32_t *elems = (uint32_t *)dst;
		SpiChnPutC(chn, 0);
		SpiChnGetRov(chn, 1);
		for (ui = 0; ui < n; ++ui)
			elems[ui] = SpiChnGetC(chn);
	} else if (_SpiMapTbl[chn]->con.MODE16) {
		uint16_t *elems = (uint16_t *)dst;
		SpiChnPutC(chn, 0);
		SpiChnGetRov(chn, 1);
		for (ui = 0; ui < n; ++ui)
			elems[ui] = (uint16_t)SpiChnGetC(chn);
	} else {    /* 8-bit mode */
		uint8_t *elems = (uint8_t *)dst;
		SpiChnPutC(chn, 0);
		SpiChnGetRov(chn, 1);
		for (ui = 0; ui < n; ++ui)
			elems[ui] = (uint8_t)SpiChnGetC(chn);
	}
}
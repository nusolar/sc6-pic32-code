#include "spi.h"
#include <../../pic32-libs/peripheral/spi/source/_spi_map_tbl.h>

void
nu_spi_rx(const struct nu_spi *s, void *dst, size_t n)
{
    u32 ui;

    /* Clear receive buffer */
    while (SpiChnRxBuffCount(s->chn))
        SpiChnReadC(s->chn);

    /*
    for (ui = 0; ui < n; ++ui) {
        SpiChnPutC(s->chn, 0x00);
        SpiChnGetRov(s->chn, 1);
        bytes[ui] = (u8)SpiChnGetC(s->chn);
    }
     */

    if (_SpiMapTbl[s->chn]->con.MODE32) {
        u32 *elems = (u32 *)dst;
        SpiChnPutC(s->chn, 0);
        SpiChnGetRov(s->chn, 1);
        for (ui = 0; ui < n; ++ui)
            elems[ui] = SpiChnGetC(s->chn);
    } else if (_SpiMapTbl[s->chn]->con.MODE16) {
        u16 *elems = (u16 *)dst;
        SpiChnPutC(s->chn, 0);
        SpiChnGetRov(s->chn, 1);
        for (ui = 0; ui < n; ++ui)
            elems[ui] = (u16)SpiChnGetC(s->chn);
    } else {    /* 8-bit mode */
        u8 *elems = (u8 *)dst;
        SpiChnPutC(s->chn, 0);
        SpiChnGetRov(s->chn, 1);
        for (ui = 0; ui < n; ++ui)
            elems[ui] = (u8)SpiChnGetC(s->chn);
    }
}

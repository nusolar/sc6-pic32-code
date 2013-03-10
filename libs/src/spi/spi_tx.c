#include "spi.h"
#include "timer.h"
#include <../../pic32-libs/peripheral/spi/source/_spi_map_tbl.h>

static ALWAYSINLINE u32
spi_chn_get_bitrate(SpiChannel chn)
{
    u32 clk_div = (_SpiMapTbl[chn]->brg+1)*2;
    u32 bitrate = PBUS_FREQ_HZ/clk_div;
    return bitrate;

    /*
	unsigned int	clkDiv=srcClk/spiClk;

	_SpiMapTbl[chn]->brg=clkDiv/2-1;

	return srcClk/clkDiv;
     */
}

static ALWAYSINLINE void
spi_wait_busy(const struct nu_spi *s)
{
    u32 bit_time_ns = 1000000000/spi_chn_get_bitrate(s->chn);

    while (SpiChnIsBusy(s->chn))
        Nop();

    /* workaround for silicone issue 6 in http://ww1.microchip.com/downloads/en/DeviceDoc/80480J.pdf :
     * The SPIBUSY and SRMT bits assert 1 bit time before the end of the transaction.
     */
    delay_ns(bit_time_ns);
}

void
nu_spi_tx(const struct nu_spi *s, const void *src, size_t n,
    enum nu_spi_tx_options opt)
{
    u32 ui;

    if (opt & NU_SPI_TX_WAIT_START)
        spi_wait_busy(s);

    if (_SpiMapTbl[s->chn]->con.MODE32) {
        const u32 *elems = (const u32 *)src;
        SpiChnWriteC(s->chn, elems[0]);
        for (ui = 1; ui < n; ++ui)
            SpiChnPutC(s->chn, elems[ui]);
    } else if (_SpiMapTbl[s->chn]->con.MODE16) {
        const u16 *elems = (const u16 *)src;
        SpiChnWriteC(s->chn, elems[0]);
        for (ui = 1; ui < n; ++ui)
            SpiChnPutC(s->chn, elems[ui]);
    } else {    /* 8-bit mode */
        const u8 *elems = (const u8 *)src;
        SpiChnWriteC(s->chn, elems[0]);
        for (ui = 1; ui < n; ++ui)
            SpiChnPutC(s->chn, elems[ui]);
    }

    if (opt & NU_SPI_TX_WAIT_END)
        spi_wait_busy(s);
}

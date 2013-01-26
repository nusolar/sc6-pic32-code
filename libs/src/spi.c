#include "compiler.h"
#include "nu32.h"
#include "param.h"
#include "spi.h"
#include "timer.h"

void
spi_setup(const struct spi *s, u32 bitrate, SpiOpenFlags oflags)
{
    u32 pbus_freq_hz = HZ/((uint32_t)(1 << OSCCONbits.PBDIV));
    SpiChnOpen(s->chn, oflags & ~SPI_OPEN_MSSEN, pbus_freq_hz/bitrate);
}

void
spi_cs_setup(const struct spi *s, u32 bitrate, SpiOpenFlags oflags)
{
    pin_set_digital_out(&(s->cs));
    spi_drive_cs_high(s);
    spi_setup(s, bitrate, oflags);
}

void
spi_tx(const struct spi *s, const void *src, size_t n)
{
    u32 ui;
    const byte *bytes = (const byte *)src;

    if (AUTO_CS_ENABLED(s))
        spi_drive_cs_low(s);
    
    for (ui = 0; ui < n; ++ui)
        SpiChnPutC(s->chn, bytes[ui]);

#if 0
    delay_us(200);  /* must be at least 100us to work with Nokia 5110 LCD */
#endif

    delay_ns(40);

    if (AUTO_CS_ENABLED(s))
        spi_drive_cs_high(s);
}

void
spi_rx(const struct spi *s, void *dst, size_t n)
{
    u32 ui;
    byte *bytes = (byte *)dst;
    
    if (AUTO_CS_ENABLED(s))
        spi_drive_cs_low(s);

    /* Clear receive buffer */
    while (SpiChnRxBuffCount(s->chn))
        SpiChnReadC(s->chn);

    for (ui = 0; ui < n; ++ui) {
        SpiChnPutC(s->chn, 0x00);
        SpiChnGetRov(s->chn, 1);
        bytes[ui] = (byte)SpiChnGetC(s->chn);
    }

    delay_us(20);
    if (AUTO_CS_ENABLED(s))
        spi_drive_cs_high(s);
}

#ifndef NU_SPI_H
#define NU_SPI_H

#include <peripheral/spi.h>
#include <string.h>
#include "compiler.h"
#include "nu_types.h"
#include "pinctl.h"
#include "timer.h"

/* auto chip select control */
enum auto_cs {
    AUTO_CS_DISABLE = 0,
    AUTO_CS_ENABLE
};

struct spi {
    struct pin cs;  /* chip select pin */
    SpiChannel chn;
    enum auto_cs auto_cs;
};

#define SPI_CS_INIT(_cs_ltr, _cs_num, _chn, _auto_cs)   \
    {                                                   \
    .cs  = PIN_INIT(_cs_ltr, _cs_num),                  \
    .chn = (_chn),                                      \
    .auto_cs = (_auto_cs)                               \
    }
#define SPI_INIT(chn)   \
    SPI_CS_INIT(IOPORT_A, BIT_0, chn, AUTO_CS_DISABLE)

#define SPI_CS(name, _cs_ltr, _cs_num, _chn, _auto_cs)  \
    struct spi name = SPI_CS_INIT(_cs_ltr, _cs_num, _chn, _auto_cs)
#define SPI(name, chn)  \
    struct spi name = SPI_INIT(chn)

static INLINE void
INIT_SPI_CS(struct spi *s, IoPortId cs_ltr, u32 cs_num, SpiChannel chn, enum auto_cs auto_cs)
{
    INIT_PIN(&(s->cs), cs_ltr, cs_num);
    s->chn = chn;
    s->auto_cs = auto_cs;
}

static INLINE void
INIT_SPI(struct spi *s, SpiChannel chn)
{
    INIT_SPI_CS(s, IOPORT_A, BIT_0, chn, AUTO_CS_DISABLE);
}

#define AUTO_CS_ENABLED(spi)    \
    (AUTO_CS_ENABLE == (spi)->auto_cs)
#define AUTO_CS_DISABLED(spi)   \
    (AUTO_CS_DISABLE == (spi)->auto_cs)

static ALWAYSINLINE void
spi_drive_cs_high(const struct spi *s)
{
    /* delay_us(5); */
    pin_set(&(s->cs));
    delay_us(5);
}

static ALWAYSINLINE void
spi_drive_cs_low(const struct spi *s)
{
    pin_clear(&(s->cs));
    delay_us(5);
}

void
spi_tx(const struct spi *s, const void *src, size_t n);

static ALWAYSINLINE void
spi_puts(const struct spi *s, const char *str)
{
    spi_tx(s, str, strlen(str));
}

void
spi_rx(const struct spi *s, void *dst, size_t n);

void
spi_setup(const struct spi *s, u32 bitrate, SpiOpenFlags oflags);

void
spi_cs_setup(const struct spi *s, u32 bitrate, SpiOpenFlags oflags);

#endif

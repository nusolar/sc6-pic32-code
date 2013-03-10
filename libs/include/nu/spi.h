#ifndef NU_SPI_H
#define NU_SPI_H 1

#include "compiler.h"
#include "nu_types.h"
#include "pinctl.h"
#include <peripheral/spi.h>

enum nu_spi_options {
    NU_SPI_DEFAULT = 0
};

struct nu_spi {
    struct nu_pin cs;  /* chip select pin */
    SpiChannel chn;
    enum nu_spi_options opt;
};

#define NU_SPI_CS_INIT(cs_ltr, cs_num, chn, opt) \
    { \
    NU_PIN_INIT(cs_ltr, cs_num), \
    (chn), \
    (opt) \
    }
#define NU_SPI_INIT(chn, opt) \
    NU_SPI_CS_INIT(IOPORT_A, BIT_0, chn, opt)

#define NU_SPI_CS(name, cs_ltr, cs_num, chn, opt)  \
    struct nu_spi name = NU_SPI_CS_INIT(cs_ltr, cs_num, chn, opt)
#define NU_SPI(name, chn, opt)  \
    struct nu_spi name = NU_SPI_INIT(chn, opt)

static INLINE void
NU_INIT_SPI_CS(struct nu_spi *s, IoPortId cs_ltr, u32 cs_num, SpiChannel chn,
        enum nu_spi_options opt)
{
    NU_INIT_PIN(&(s->cs), cs_ltr, cs_num);
    s->chn = chn;
    s->opt = opt;
}

static INLINE void
NU_INIT_SPI(struct nu_spi *s, SpiChannel chn, enum nu_spi_options opt)
{
    NU_INIT_SPI_CS(s, IOPORT_A, BIT_0, chn, opt);
}

static ALWAYSINLINE void
nu_spi_drive_cs_high(const struct nu_spi *s)
{
    nu_pin_set(&(s->cs));
}

static ALWAYSINLINE void
nu_spi_drive_cs_low(const struct nu_spi *s)
{
    nu_pin_clear(&(s->cs));
}

enum nu_spi_tx_options {
    NU_SPI_TX_WAIT_START = 1<<0,    /* wait until current transmission complete before starting */
    NU_SPI_TX_WAIT_END   = 1<<1     /* wait until current transmission complete before ending */
};

void
nu_spi_tx(const struct nu_spi *s, const void *src, size_t n,
        enum nu_spi_tx_options opt);

static ALWAYSINLINE void
nu_spi_puts(const struct nu_spi *s, const char *str, enum nu_spi_tx_options opt)
{
    nu_spi_tx(s, str, strlen(str), opt);
}

void
nu_spi_rx(const struct nu_spi *s, void *dst, size_t n);

void
nu_spi_setup(const struct nu_spi *s, u32 bitrate, SpiOpenFlags oflags);

void
nu_spi_cs_setup(const struct nu_spi *s, u32 bitrate, SpiOpenFlags oflags);

#endif

#ifndef NU_SPI_H
#define NU_SPI_H 1

#include "nu/compiler.h"
#include "nu/nu_types.h"
#include "nu/pinctl.h"
#include "nu/platform.h"

#if BOOST_PP_VARIADICS
# define NU_SPI_CS_INIT(pin, ...) {pin, {__VA_ARGS__}}
# define NU_SPI_INIT(...) {NU_PIN_INIT(NU_PIN_DEFAULT), {__VA_ARGS__}}
# define NU_INIT_SPI_CS(s, cs, ...) \
    __nu_init_spi_cs(s, cs, __VA_ARGS__)
# define NU_INIT_SPI(s, ...) \
    NU_INIT_SPI_CS(s, NU_PIN_DEFAULT, __VA_ARGS__)
#endif

#if NU_ARCH == NU_ARCH_PIC32MX
# include "spi_pic32mx.h"
#endif

struct nu_spi;

#define NU_SPI_CS(name, cs_pin, spi)  \
    struct nu_spi name = NU_SPI_CS_INIT(cs_pin, spi)
#define NU_SPI(name, spi)  \
    struct nu_spi name = NU_SPI_INIT(spi)

static INLINE void
NU_INIT_SPI_CS(struct nu_spi *s, struct nu_pin *cs_pin, SpiChannel chn,
        enum nu_spi_options opt)
{
    s->cs = *cs_pin;
    s->chn = chn;
    s->opt = opt;
}

static INLINE void
NU_INIT_SPI(struct nu_spi *s, SpiChannel chn, enum nu_spi_options opt)
{
    NU_PIN(tmp, NU_PIN_DEFAULT);
    NU_INIT_SPI_CS(s, &tmp, chn, opt);
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

#ifndef NU_SPI_H
#define NU_SPI_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include "nu/pinctl.h"
/*
 * NU_SPI_PLATFORM_INIT
 * NU_INIT_SPI_PLATFORM
 * nu_init_spi_platform_args_t
 * (optional) nu_init_spi_platform_defaults
 * nu_spi_platform_setup
 * nu_spi_platform_setup_args_t
 * (optional) nu_spi_platform_setup_defaults
 */
#include "nu/platform/spi.h"

struct nu_spi {
    struct nu_pin cs;
    struct nu_spi_platform platform;
};
#define NU_SPI_INIT(cs, platform) {cs, platform}
#define NU_SPI(name, cs, platform) \
    struct nu_spi name = NU_SPI_INIT(cs, platform)

static ALWAYSINLINE void
NU_INIT_SPI(struct nu_spi *s,
        const nu_init_pin_platform_args_t *pa,
        const nu_init_spi_platform_args_t *a)
{
    NU_INIT_PIN_PLATFORM(&(s->cs.platform), pa);
    NU_INIT_SPI_PLATFORM(&(s->platform), a);
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

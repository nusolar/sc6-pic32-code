#ifndef NU_SPI_H
#define NU_SPI_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include "nu/pinctl.h"

struct nu_spi_platform;
struct nu_spi_platform_setup_args;

struct nu_spi_platform_ops {
    void (*setup)   (struct nu_spi_platform *p, u32 bitrate,
            const struct nu_spi_platform_setup_args *args);
    s32 (*putchar)  (const struct nu_spi_platform *p, s32 c);
    s32 (*getchar)  (const struct nu_spi_platform *p);
};

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
nu_spi_cs_high(const struct nu_spi *s)
{
    nu_pin_set(&(s->cs));
}

static ALWAYSINLINE void
nu_spi_cs_low(const struct nu_spi *s)
{
    nu_pin_clear(&(s->cs));
}

void
nu_spi_setup(struct nu_spi *s, u32 bitrate,
        const struct nu_spi_platform_setup_args *args);

size_t
nu_spi_tx(const struct nu_spi *s, const void *src, size_t n);

size_t
nu_spi_rx(const struct nu_spi *s, void *dst, size_t n);

#endif

#ifndef NU_SPI_PIC32MX_H
#define NU_SPI_PIC32MX_H 1

#include "nu/compiler.h"
#include <peripheral/spi.h>

typedef struct nu_spi_platform {
    SpiChannel chn;
} nu_init_spi_platform_args_t;
#define NU_SPI_PLATFORM_INIT(chn) { chn }

static ALWAYSINLINE void
NU_INIT_SPI_PLATFORM(struct nu_spi_platform *p,
        const nu_init_spi_platform_args_t *args)
{
    p->chn = args->chn;
}

const struct nu_spi_platform_setup_args {
    int foo;
} nu_spi_platform_setup_defaults = {
    0
};

void
nu_spi_platform_setup(struct nu_spi_platform *p,
    const struct nu_spi_platform_setup_args *args);

#endif

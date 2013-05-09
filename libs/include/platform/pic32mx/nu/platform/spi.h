#ifndef NU_SPI_PIC32MX_H
#define NU_SPI_PIC32MX_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include <peripheral/spi.h>

typedef struct nu_spi_platform {
    SpiChannel chn;
} nu_init_spi_platform_args_t;
#define NU_SPI_PLATFORM_INIT(chn) { chn }

#define NU_SPI_CHANNEL1 { SPI_CHANNEL1 }
#define NU_SPI_CHANNEL2 { SPI_CHANNEL2 }
#define NU_SPI_CHANNEL3 { SPI_CHANNEL3 }
#define NU_SPI_CHANNEL4 { SPI_CHANNEL4 }

static ALWAYSINLINE void
NU_INIT_SPI_PLATFORM(struct nu_spi_platform *p,
        const nu_init_spi_platform_args_t *args)
{
    p->chn = args->chn;
}

static const struct nu_spi_platform_setup_args {
    SpiOpenFlags oflags;
} nu_spi_platform_setup_defaults = {
    (SpiOpenFlags)SPI_OPEN_MSTEN|SPI_OPEN_MODE8,
};

#endif

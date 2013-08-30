#ifndef NU_PLATFORM_SPI_H
#define NU_PLATFORM_SPI_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/spi.h"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC
#error "No generic SPI code!"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX
#include <peripheral/spi.h>

extern const struct nu_spi_platform_ops nu_spi_platform_ops;

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

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_SPI_H */

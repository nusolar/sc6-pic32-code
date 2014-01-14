#include "nu/platform/spi.h"
#include "nu/peripheral/spi.h"
#include "nu/timer.h"

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/spi.c"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC
#error "No generic SPI code!"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX
#include <peripheral/spi.h>

static void
nu__Spi__platform_setup(struct nu__Spi__Platform *p, u32 bitrate,
    const struct nu__Spi__PlatformSetupArgs *args)
{
    SpiChnOpen(p->chn, args->oflags, NU_PBUS_FREQ_HZ/bitrate);
}

static s32
nu__Spi__platform_putchar(const struct nu__Spi__Platform *p, s32 c)
{
    SpiChnPutC(p->chn, (u32)c);
    return c;
}

static s32
nu__Spi__platform_getchar(const struct nu__Spi__Platform *p)
{
    return (s32)SpiChnReadC(p->chn);
}

const struct nu__Spi__PlatformOps nu__Spi__platform_ops = {
    nu__Spi__platform_setup,      /* setup */
    nu__Spi__platform_putchar,    /* putchar */
    nu__Spi__platform_getchar     /* getchar */
};

#endif /* NU_PLATFORM switch */

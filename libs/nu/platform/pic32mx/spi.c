#include "nu/platform/spi.h"
#include "nu/spi.h"
#include "nu/timer.h"
#include <peripheral/spi.h>

static void
nu_spi_platform_setup(struct nu_spi_platform *p, u32 bitrate,
    const struct nu_spi_platform_setup_args *args)
{
    SpiChnOpen(p->chn, args->oflags, NU_PBUS_FREQ_HZ/bitrate);
}

static s32
nu_spi_platform_putchar(const struct nu_spi_platform *p, s32 c)
{
    SpiChnPutC(p->chn, (u32)c);
    return c;
}

static s32
nu_spi_platform_getchar(const struct nu_spi_platform *p)
{
    return (s32)SpiChnReadC(p->chn);
}

const struct nu_spi_platform_ops nu_spi_platform_ops = {
    nu_spi_platform_setup,      /* setup */
    nu_spi_platform_putchar,    /* putchar */
    nu_spi_platform_getchar     /* getchar */
};

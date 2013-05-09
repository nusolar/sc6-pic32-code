#include "nu/spi.h"
#include "nu/wdt.h"

#undef putchar
#undef getchar

void
nu_spi_setup(struct nu_spi *s, u32 bitrate,
        const struct nu_spi_platform_setup_args *args)
{
    nu_pin_set_digital_out(&(s->cs));
    nu_spi_cs_high(s);
    if (nu_spi_platform_ops.setup)
        nu_spi_platform_ops.setup(&(s->platform), bitrate, args);
}

size_t
nu_spi_tx(const struct nu_spi *s, const void *src, size_t n)
{
    u32 ui;
    if (!nu_spi_platform_ops.putchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        nu_wdt_clear();
        if (nu_spi_platform_ops.putchar(&(s->platform), ((const u8 *)src)[ui]) < 0)
            break;
    }
    return ui;
}

size_t
nu_spi_rx(const struct nu_spi *s, void *dst, size_t n)
{
    u32 ui;
    if (!nu_spi_platform_ops.getchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        s32 c;
        if ((c = nu_spi_platform_ops.getchar(&(s->platform))) < 0)
            break;
        ((u8 *)dst)[ui] = (u8)c;
    }
    return ui;
}

#include "nu/peripheral/spi.h"
#include "nu/wdt.h"

#undef putchar
#undef getchar

void
nu__Spi__setup(struct nu__Spi *s, u32 bitrate,
        const struct nu__Spi__PlatformSetupArgs *args)
{
    nu__Pin__set_digital_out(&(s->cs));
    nu__Spi__cs_high(s);
    if (nu__Spi__platform_ops.setup)
        nu__Spi__platform_ops.setup(&(s->platform), bitrate, args);
}

size_t
nu__Spi__tx(const struct nu__Spi *s, const void *src, size_t n)
{
    u32 ui;
    if (!nu__Spi__platform_ops.putchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        nu__WDT__clear();
        if (nu__Spi__platform_ops.putchar(&(s->platform), ((const u8 *)src)[ui]) < 0)
            break;
    }
    return ui;
}

size_t
nu__Spi__rx(const struct nu__Spi *s, void *dst, size_t n)
{
    u32 ui;
    if (!nu__Spi__platform_ops.getchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        s32 c;
        if ((c = nu__Spi__platform_ops.getchar(&(s->platform))) < 0)
            break;
        ((u8 *)dst)[ui] = (u8)c;
    }
    return ui;
}

#include "nu/ad7685.h"
#include "nu/errorcodes.h"
#include "nu/timer.h"
#include <sys/endian.h>
#include <alloca.h>

#define SPI_OFLAGS (SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON)
static const u32 spi_bitrate = 100000;

#define drive_cnv_high(a)   nu_pin_set(&((a)->convert_pin))
#define drive_cnv_low(a)    nu_pin_clear(&((a)->convert_pin))

s32 MUST_CHECK
nu_ad7685_setup(const struct nu_ad7685 *a)
{
    if ((NU_AD7685_FOUR_WIRE & a->opt && NU_AD7685_NO_BUSY_INDICATOR & a->opt) ||
            (NU_AD7685_CHAIN_MODE & a->opt && NU_AD7685_BUSY_INDICATOR & a->opt))
        return -EINVAL;
    nu_pin_set_digital_out(&(a->convert_pin));
    nu_spi_setup(&(a->spi), spi_bitrate, (SpiOpenFlags) SPI_OFLAGS);
    return 0;
}

static void
read_uv(const struct nu_ad7685 *a, u32 *dst)
{
    u32 ui;
    u16 *buf = (u16 *)alloca(sizeof(*buf) * a->num_devices);

    nu_spi_rx(&(a->spi), &buf, sizeof(*buf) * a->num_devices);
    for (ui = 0; ui < a->num_devices; ++ui) {
        /* swap byte order ... */
        /* buf[ui] = bswap_u16(buf[ui]); */
        buf[ui] = betoh16(buf[ui]);
        /* then compute the actual voltage (microvolts) */
        dst[ui] = (u32)((5000000 * (u64)buf[ui])>>16);
    }
}

/* gets the actual voltage reading(s) (not raw data) */
void
nu_ad7685_convert_read_uv(const struct nu_ad7685 *a, u32 *dst)
{
    if (NU_AD7685_FOUR_WIRE & a->opt && NU_AD7685_BUSY_INDICATOR & a->opt)
        nu_spi_drive_cs_high(&(a->spi));

    /* start conversion */
    drive_cnv_high(a);
    nu_delay_ns(100);  /* .1 us */

    if (NU_AD7685_BUSY_INDICATOR & a->opt) {
        if (NU_AD7685_THREE_WIRE & a->opt)
            drive_cnv_low(a);
        else if (NU_AD7685_FOUR_WIRE & a->opt)
            nu_spi_drive_cs_low(&(a->spi));
    }

    nu_delay_ns(2300); /* 2.3 us */

    if (NU_AD7685_THREE_WIRE & a->opt && NU_AD7685_NO_BUSY_INDICATOR & a->opt)
        drive_cnv_low(a);

    /* read in the actual voltage reading(s) over SPI */
    read_uv(a, dst);

    drive_cnv_low(a);
    nu_delay_us(5);
}

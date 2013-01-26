#include "ad7685.h"
#include "byteorder.h"
#include "errorcodes.h"
#include "timer.h"

#define SPI_OFLAGS SPI_OPEN_CKE_REV|SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON
static const u32 spi_bitrate = 100000;

#define drive_cnv_high(a)   pin_set(&((a)->convert_pin))
#define drive_cnv_low(a)    pin_clear(&((a)->convert_pin))

s32 MUST_CHECK
ad7685_setup(const struct ad7685 *a)
{
    if (((FOUR_WIRE == wire_config && NO_BUSY_INDICATOR == busy_indic) ||
        (CHAIN_MODE == wire_config && USE_BUSY_INDICATOR == busy_indic)))
        return -EINVAL;
    pin_set_digital_out(&(a->convert_pin));
    spi_setup(&(a->spi), spi_bitrate, SPI_OFLAGS);
}

static void
read_uv(const struct ad7685 *a, u32 *dst)
{
    u32 ui;
    u16 buf[a->num_devices];

    spi_rx(&(a->spi), &buf, sizeof(buf));
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
ad7685_convert_read_uv(const struct ad7685 *a, u32 *dst)
{
    if (FOUR_WIRE == a->wire_config && USE_BUSY_INDICATOR == a->busy_indic)
        spi_drive_cs_high(&(a->spi));
    
    /* start conversion */
    drive_cnv_high(a);
    delay_ns(100);  /* .1 us */

    if (USE_BUSY_INDICATOR == a->busy_indic) {
        if (THREE_WIRE == a->wire_config)
            drive_cnv_low(a);
        else if (FOUR_WIRE == a->wire_config)
            spi_drive_cs_low(&(a->spi));
    }

    delay_ns(2300); /* 2.3 us */

    if (THREE_WIRE == a->wire_config && NO_BUSY_INDICATOR == a->busy_indic)
        drive_cnv_low(a);

    /* read in the actual voltage reading(s) over SPI */
    read_uv(a, dst);

    drive_cnv_low(a);
    delay_us(5);
}

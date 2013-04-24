#include "spi.h"
#include "param.h"

void
nu_spi_setup(const struct nu_spi *s, u32 bitrate, SpiOpenFlags oflags)
{
    SpiChnOpen(s->chn, oflags, NU_PBUS_FREQ_HZ/bitrate);
}

void
nu_spi_cs_setup(const struct nu_spi *s, u32 bitrate, SpiOpenFlags oflags)
{
    nu_pin_set_digital_out(&(s->cs));
    nu_spi_drive_cs_high(s);
    nu_spi_setup(s, bitrate, oflags);
}

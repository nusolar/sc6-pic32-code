#ifndef NU_SPI_PIC32MX_H
#define NU_SPI_PIC32MX_H 1

#include <peripheral/spi.h>

struct nu_spi {
    struct nu_pin cs;
    SpiChannel chn;
};

#ifndef NU_SPI_INIT
# define NU_SPI_INIT(chn) {NU_PIN_INIT(NU_PIN_DEFAULT), {chn}}
#endif
#ifndef NU_INIT_SPI
# define NU_INIT_SPI(spi, chn) __nu_init_spi(s, chn)
#endif
#ifndef NU_SPI_CS_INIT
# define NU_SPI_CS_INIT(cs, chn) {cs, {chn}}
#endif
#ifndef NU_INIT_SPI_CS
# define NU_INIT_SPI_CS(s, cs, chn) __nu_init_spi_cs(s, cs, chn)
#endif

#endif

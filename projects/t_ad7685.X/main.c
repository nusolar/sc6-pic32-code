#include "nu32.h"
#include "ad7685.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct ad7685 adc; struct ad7685 *adcp = &adc;
    float voltage;

    nu32_init(80000000);

    ad7685_new(adcp, SPI_CHANNEL4,
            IOPORT_F, BIT_12,
            1,
            CHAIN_MODE, NO_BUSY_INDICATOR);

    adcp->op->convertAndReadVolts(adcp, &voltage);
    voltage = voltage;

    while (1)
        ;   /* do nothing */

    return 0;
}
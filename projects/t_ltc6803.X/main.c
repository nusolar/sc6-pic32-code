#include "nu32.h"
#include "ltc6803.h"

#include "common_pragmas.h"

int32_t
main(void)
{
    struct ltc6803 ltc; struct ltc6803 *ltcp = &ltc;
    union BpsConfig cfg = {0};
    float voltages[12] = {0};
    int32_t err;
    uint32_t ui;

    nu32_init(80000000);

    /* LTC6803 configuration: measurement only mode,
     * 13ms minimum measurement time */
    cfg.cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
    cfg.vov = convertOVLimit(20);
    cfg.vuv = convertUVLimit(5);

    if((err = ltc6803_new(ltcp, SPI_CHANNEL1,
            IOPORT_D, BIT_9,
            1, &cfg)) < 0)
        return err;

    delay_us(20);

    ltcp->op->startVoltageConversion(ltcp);

    delay_us(20);

    ltcp->op->readVolts(ltcp, voltages);

    for (ui = 0; ui < ARRAY_SIZE(voltages); ++ui) {
        float v = voltages[ui];
        Nop();
    }

    while (1)
        ;   /* do nothing */

    return 0;
}
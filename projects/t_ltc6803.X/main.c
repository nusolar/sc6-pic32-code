#include "nu32.h"
#include "ltc6803.h"

#include "common_pragmas.h"

static const IoPortId      MAIN_RELAY_PIN_LTR      = IOPORT_D;
static const uint32_t      MAIN_RELAY_PIN_NUM      = BIT_2;

int32_t
main(void)
{
    struct ltc6803 ltc; struct ltc6803 *ltcp = &ltc;
    union BpsConfig cfg[3] = {0};
    float voltages[36] = {0};
    int32_t err;
    uint32_t ui;

    PORTSetPinsDigitalOut(MAIN_RELAY_PIN_LTR, MAIN_RELAY_PIN_NUM);
    PORTSetBits(MAIN_RELAY_PIN_LTR, MAIN_RELAY_PIN_NUM);

    nu32_init(80000000);

    /* LTC6803 configuration: measurement only mode,
     * 13ms minimum measurement time */
    cfg[0].cfgr0 = WDT | LVLPL | CDC_MSMTONLY;
    cfg[0].vov = convertOVLimit(15);
    cfg[0].vuv = convertUVLimit(6);
    cfg[2] = cfg[1] = cfg[0];

    if((err = ltc6803_new(ltcp, SPI_CHANNEL1,
            IOPORT_D, BIT_9,
            ARRAY_SIZE(cfg), cfg)) < 0)
        return err;

    delay_us(20);

    ltcp->op->startVoltageConversion(ltcp);

    delay_ms(14);

    ltcp->op->readVolts(ltcp, voltages);

    for (ui = 0; ui < ARRAY_SIZE(voltages); ++ui) {
        float v = voltages[ui];
        Nop();
    }

    while (1)
        ;   /* do nothing */

    return 0;
}
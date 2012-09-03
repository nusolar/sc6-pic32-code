#include "ltc6803.h"
#include "nu32.h"

#include "common_pragmas.h"

static const IoPortId      MAIN_RELAY_PIN_LTR      = IOPORT_D;
static const uint32_t      MAIN_RELAY_PIN_NUM      = BIT_2;

static const SpiChannel    LTC6803_SPI_CHN         = SPI_CHANNEL4;
static const IoPortId      LTC6803_CS_PIN_LTR      = IOPORT_D;
static const uint32_t      LTC6803_CS_PIN_NUM      = BIT_9;

#define                    LTC6803_COUNT            3

static struct ltc6803      ltc,        *ltcp        = &ltc;

int32_t
main(void)
{
    union BpsConfig cfg[1];

    nu32_init(80000000);

    PORTSetPinsDigitalOut(MAIN_RELAY_PIN_LTR, MAIN_RELAY_PIN_NUM);
    PORTSetBits(MAIN_RELAY_PIN_LTR, MAIN_RELAY_PIN_NUM);

    ClearWDT();

    memset(cfg, 0, sizeof(cfg));

    /* LTC6803 configuration: measurement only mode,
     * 13ms minimum measurement time */
    cfg[0].cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
    // cfg[2] = cfg[1] = cfg[0];

//  cfg[0].cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
//    cfg[0].vov = convertOVLimit(OVER_VOLTAGE);
//    cfg[0].vuv = convertUVLimit(UNDER_VOLTAGE);
//  cfg[2] = cfg[1] = cfg[0];

    IF_ERR(ltc6803_new(ltcp, LTC6803_SPI_CHN, LTC6803_CS_PIN_LTR,
                    LTC6803_CS_PIN_NUM, LTC6803_COUNT, cfg),
                REP_CRITICAL, "ltc6803_new")
        Nop();

    ltcp->op->post(ltcp);

    Nop();

    while (1)
        ; /* do nothing */
}
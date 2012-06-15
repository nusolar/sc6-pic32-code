#include "../bms.X/bms.h"

int32_t
main(void)
{
    int32_t err = 0;

    nu32_init(115200);

    struct nokia5110    display;
    struct nokia5110    *dp         = &display;
    struct ltc6803      ltc;
    struct ltc6803      *ltcp       = &ltc;

    if ((err = nokia5110_new(dp, SPI_CHANNEL3, IOPORT_E, BIT_2, IOPORT_E, BIT_1,
                            IOPORT_E, BIT_0)))
        ;

    union BpsConfig cfg[1];
    memset(&cfg, 0, sizeof(cfg));
    cfg[0].cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
    cfg[0].vov = convertOVLimit(OVER_VOLTAGE);
    cfg[0].vuv = convertUVLimit(UNDER_VOLTAGE);

    /* LTC6803 pins: SPI channel 4; CS pin G9 */
    if ((err = ltc6803_new(ltcp, SPI_CHANNEL4, IOPORT_G, BIT_9, 1, cfg)))
        ;

    /* LTC6803 Power-On Self Test */
    err = ltcp->op->post(ltcp);


    float voltages[12];
    memset(voltages, 0, sizeof(voltages));

    /* Determine when to perform open-wire check */
    double openWireStart = readTimer();
    double openWireTrigger;

    /* Open-wire check: arrays to compare*/
    uint16_t openWire[1];
    memset(openWire, 0, sizeof(openWire));
    float voltages1[12];
    memset(voltages1, 0, sizeof(voltages1));



    int count = 0;
    while(1) {
        

        /* Get cell voltage readings */
        ltcp->op->startVoltageConversion(ltcp);
        delay_ms(16);
        ltcp->op->readVolts(ltcp, voltages);

        /* Check for open sense wires */
        openWireTrigger = readTimer();
        if (openWireTrigger - openWireStart > OPEN_WIRE_INTERVAL) {
            count++;
            memset(openWire, 0, sizeof(openWire));  
            ltcp->op->startOpenWireConversion(ltcp);
            delay_ms(16);
            ltcp->op->readVolts(ltcp, voltages1);
            ltcp->op->checkOpenWire(ltcp, openWire, voltages1);
            openWireStart = readTimer();
        }

        if (openWireTrigger - openWireStart < 0) {
            openWireStart = readTimer();
        }

        dp->op->clear(dp);
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "%d", count);
        dp->op->gotoXY(dp, 0, 1);
        dp->op->printf(dp, "%X", openWire[0]);
        dp->op->gotoXY(dp, 0, 2);
        dp->op->printf(dp, "%f", voltages[0]);
        
  
    }
    exit(EXIT_SUCCESS);
}

int32_t
handleLastReset(void)
{
    if (isPOR()) {              /*
                                 * note that a power-on reset sets BOTH the
                                 * POR and BOR bits (per the pic32 reference
                                 * manual), so the power-on reset condition
                                 * must be checked BEFORE the brown-out reset
                                 * (BOR) condition
                                 */
        mClearPORFlag();
        mClearBORFlag();

        report(REP_INFO, "POR reset");

        return POWER_ON_RESET;
    } else if (isBOR()) {
        mClearBORFlag();

        report(REP_WARNING, "BOR reset");

        return BROWNOUT_RESET;
    } else if (isLVD()) {       /* low voltage detect reset
                                 * It seems that the PIC32 libraries don't
                                 * actually support this, as reset.h has this
                                 * definition:
                                 * #define isLVD()							0
                                 */
        report(REP_WARNING, "LVD reset");

        return LOW_VOLTAGE_RESET;
    } else if (isMCLR()) {      /* master clear (reset) pin reset */
        mClearMCLRFlag();

        report(REP_INFO, "MCLR reset");

        return MASTER_CLEAR_RESET;
    } else if (isWDTTO()) {     /* watchdog timer reset */
        mClearWDTOFlag();

        report(REP_ERROR, "WDT reset");

        return WDT_RESET;
    } else if (mGetSWRFlag()) { /* software reset */
        mClearSWRFlag();

        report(REP_WARNING, "SWR reset");

        return SOFTWARE_RESET;
    } else if (mGetCMRFlag()) { /* config mismatch reset */
        mClearCMRFlag();

        report(REP_CRITICAL, "CMR reset");

        return CONFIG_MISMATCH_RESET;
    };

    return 0;
}

int32_t
init_devices(struct nokia5110 *dp, struct ltc6803 *ltcp,
        struct ad7685 *adp, struct ds18b20 *dsp, struct can *canp,
        struct serial *serp)
{


    return 0;
}

void
trip(UINT64 tripcode)
{
    (void)tripcode;
    /* @FIXME Implement trip() */
}

int32_t
loadFromFlash(void *dst, size_t len)
{
    /* @TODO implementation */
    return 0;
}
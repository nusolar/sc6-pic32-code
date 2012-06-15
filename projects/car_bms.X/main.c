#include "main.h"

int32_t
main(void)
{
    uint32_t            ui;
    uint16_t            openWire[LTC6803_COUNT];
    float               voltages1[12*LTC6803_COUNT], voltages[12*LTC6803_COUNT];
    float               rawCurrents[AD7685_COUNT];
    float               batteryCurrent, arrayCurrent;
    float               temperatures[ARRAY_SIZE(DS18X20_ROMCODES)];
    float               openWireStart, openWireTrigger;
    float               temperatureStart, temperatureTrigger;
    struct flashData    fd = {0};
    union can_anyFrame  frame = {0};
    
    /* device driver declarations
     *
     * pointers are for convenience (so all operations can be written in the
     * form pointer->op->function)
     */
    struct nokia5110    display,    *dp     = &display;
    struct ad7685       adc,        *adcp   = &adc;
    struct ltc6803      ltc,        *ltcp   = &ltc;
    struct ds18x20      ds,         *dsp    = &ds;
    struct can          can,        *canp   = &can;
    struct serial       ser,        *serp   = &ser;
    struct led          led0,       *led0p  = &led0;
    struct led          led1,       *led1p  = &led1;
    
    /* timeout setting is in main.h */
    EnableWDT();

    /* setup clock, interrupts, NU32 LED pinouts, and switch input */
    nu32_init(SYS_CLK_HZ);
    
    ClearWDT();

    REPORT_ON_ERR(readFlash(&fd,sizeof(fd)), REP_WARNING, "readFlash error");

    /* initialize devices and warn on error */
    REPORT_ON_ERR(init_devices(dp, ltcp, adcp, dsp, canp, serp, led0p, led1p),
            REP_WARNING, "init_devices failed");

    /* send cause of last reset over CAN */
    sendLastResetCode(canp);
    /* report last trip */
    REPORT((REP_INFO, "last trip was code %d, module %d",
            fd.tripInfo.code, fd.tripInfo.module));

    /* LTC6803 Power-On Self Test; report and trip on error */
    REPORT_ON_ERR(ltcp->op->post(ltcp), REP_EMERGENCY, "LTC POST FAILED");

    ClearWDT();

    /* DS18x20 Presence Test */
    for (ui = 0; ui < ARRAY_SIZE(DS18X20_ROMCODES); ui++) {
        IF_ERR(dsp->op->verify(dsp, DS18X20_ROMCODES[ui]),
                REP_EMERGENCY, "DS18X20 verify failed") {
            /* @TODO Do something if result < 0? */
        } else if (!nu_errno) {
            /* Missing Sensor */
            REPORT((REP_EMERGENCY,
                    "TEMP %d, ROMCODE %02X %02X %02X %02X %02X %02X %02X MISSING",
                    ui,
                    DS18X20_ROMCODES[ui].byteArr[0],
                    DS18X20_ROMCODES[ui].byteArr[1],
                    DS18X20_ROMCODES[ui].byteArr[2],
                    DS18X20_ROMCODES[ui].byteArr[3],
                    DS18X20_ROMCODES[ui].byteArr[4],
                    DS18X20_ROMCODES[ui].byteArr[5],
                    DS18X20_ROMCODES[ui].byteArr[6]));
            TRIP_MOD(TRIP_DS18X20_MISSING, ui);
        }
    }

    ClearWDT();
    
    /* Start first temperature conversion */
    IF_ERR(dsp->op->startTempConversion(dsp, ALL_DEVICES), REP_EMERGENCY,
            nu_errno == -ENODEV ? "OW BUS FAILURE" : "TEMP CONVERT FAILED") {
        TRIP_NOMOD(nu_errno == -ENODEV ?
            TRIP_DS18X20_MISSING : TRIP_OW_BUS_FAILURE);
    }

    ClearWDT();

    /* Determine when to perform open-wire check */
    openWireStart       = (float)readTimer();
    
    /* Determine when to poll temp sensors */
    temperatureStart    = (float)readTimer();

    /* ENTER MAIN LOOP */
    while (1) {
        ClearWDT();

        /* send heartbeat */
        REPORT_ON_ERR(canp->op->tx(canp, COMMON_CAN_TX_CHN, STANDARD_ID,
                ADDR_BMSTX(HEARTBEAT), 0,
                0, &(frame.bms.tx.heartbeat),
                sizeof(frame.bms.tx.heartbeat)),
                REP_WARNING, "CAN TX FAILED");
        
        /* Perform voltage conversion and then read voltages;
         * report and trip on error */
        IF_ERR(ltcp->op->startVoltageConversion(ltcp), REP_CRITICAL,
                        "LTC VOLTAGE CONVERSION FAILED")
            TRIP_NOMOD(TRIP_OTHER);
        delay_ms(16);
        IF_ERR(ltcp->op->readVolts(ltcp, voltages), REP_CRITICAL,
                        "LTC GET VOLTS FAILED")
           TRIP_NOMOD(TRIP_OTHER);

        ClearWDT();

        /* Get current readings */
        IF_ERR(adcp->op->convertAndReadVolts(adcp, rawCurrents),
                        REP_CRITICAL, "Current acquisition failed")
            TRIP_NOMOD(TRIP_ADC_FAILURE);
        batteryCurrent  = voltageToCurrent(rawCurrents[I_SENSOR_BATT]);
        arrayCurrent    = voltageToCurrent(rawCurrents[I_SENSOR_ARRAY]);

        frame.bms.tx.current_draw.currentDraw = batteryCurrent;
        REPORT_ON_ERR(canp->op->tx(canp, COMMON_CAN_TX_CHN, STANDARD_ID,
                ADDR_BMSTX(CURRENT_DRAW), 0,
                0, &(frame.bms.tx.current_draw),
                sizeof(frame.bms.tx.current_draw)),
                REP_WARNING, "CAN TX FAILED");

        frame.bms.tx.current_trip_pt.currentTripPt = OVER_CURRENT_DISCHARGE_A;
        REPORT_ON_ERR(canp->op->tx(canp, COMMON_CAN_TX_CHN, STANDARD_ID,
                ADDR_BMSTX(CURRENT_TRIP_PT), 0,
                0, &(frame.bms.tx.current_draw),
                sizeof(frame.bms.tx.current_trip_pt)),
                REP_WARNING, "CAN TX FAILED");

        ClearWDT();

        /* Get temperature readings every "TEMP_INTERVAL" seconds */
        temperatureTrigger = (float)readTimer();
        if (temperatureTrigger - temperatureStart > TEMP_READ_INTERVAL_S) {
            /* Get temperature readings */
            for (ui = 0; ui < ARRAY_SIZE(DS18X20_ROMCODES); ui++)
                temperatures[ui] = dsp->op->readTemp(dsp, DS18X20_ROMCODES[ui]);
            /* Start next temperature conversion */
            IF_ERR(dsp->op->startTempConversion(dsp, ALL_DEVICES), REP_EMERGENCY,
                    nu_errno == -ENODEV ? "OW BUS FAILURE" : "TEMP CONVERT FAILED")
                TRIP_NOMOD(nu_errno == -ENODEV ? TRIP_DS18X20_MISSING :
                    TRIP_OW_BUS_FAILURE);
            temperatureStart = (float)readTimer();
        }
        /* Check for timer overflow; reset temperature timer if so */
        if (temperatureTrigger - temperatureStart < 0) {
            temperatureStart = (float)readTimer();
        }

        ClearWDT();

        /* Check for open sense wires */
        openWireTrigger = (float)readTimer();
        if (openWireTrigger - openWireStart > OPEN_WIRE_INTERVAL_S) {
            memset(openWire, 0, sizeof(openWire));
            ltcp->op->startOpenWireConversion(ltcp);
            delay_ms(16);
            ltcp->op->readVolts(ltcp, voltages1);
            ltcp->op->checkOpenWire(ltcp, openWire, voltages1);
            openWireStart = (float)readTimer();
        }
        /* Check for timer overflow; reset open-wire timer if so */
        if (openWireTrigger - openWireStart < 0) {
            openWireStart = (float)readTimer();
        }

        ClearWDT();

        /* Check for over-voltage/under-voltage */
        for (ui = 0; ui < MODULE_COUNT; ui++)
            if (voltages[ui] > OVER_VOLTAGE) {
                REPORT_ERR((REP_EMERGENCY, -ETRIP, "MODULE %d OVER VOLTAGE", ui));
                TRIP_MOD(TRIP_OVER_VOLTAGE, ui);
            } else if (voltages[ui] < UNDER_VOLTAGE) {
                REPORT_ERR((REP_EMERGENCY, -ETRIP, "MODULE %d UNDER VOLTAGE", ui));
                TRIP_MOD(TRIP_UNDER_VOLTAGE, ui);
            }

        ClearWDT();

        /* Check for over/under-current */
        if (batteryCurrent < OVER_CURRENT_DISCHARGE_A) {
            REPORT_ERR((REP_EMERGENCY, -ETRIP, "OVER CURRENT DISCHRG"));
            TRIP_NOMOD(TRIP_OVER_CURRENT_DISCHRG);
        } else if (batteryCurrent > OVER_CURRENT_CHARGE_A) {
            REPORT_ERR((REP_EMERGENCY, -ETRIP, "OVER CURRENT CHRG"));
            TRIP_NOMOD(TRIP_OVER_CURRENT_CHRG);
        }

        ClearWDT();

        /* Check for over-temperature/under-temperature */
        for (ui = 0; ui < ARRAY_SIZE(DS18X20_ROMCODES); ui++)
            if (temperatures[ui] > OVER_TEMP_C) {
                REPORT_ERR((REP_EMERGENCY, -ETRIP, "SENSOR %d OVER TEMP", ui));
                TRIP_MOD(TRIP_OVER_TEMP, ui);
            } else if (temperatures[ui] < UNDER_TEMP_C) {
                REPORT_ERR((REP_EMERGENCY, -ETRIP, "SENSOR %d UNDER TEMP", ui));
                TRIP_MOD(TRIP_UNDER_TEMP, ui);
            }

        ClearWDT();
    } /* main event loop */

    exit(EXIT_SUCCESS); /* should never get here */
}

enum lastReset
getLastResetCause(void)
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

        REPORT((REP_INFO, "POR reset"));

        return POWER_ON_RESET;
    } else if (isBOR()) {
        mClearBORFlag();

        REPORT((REP_WARNING, "BOR reset"));
        
        return BROWNOUT_RESET;
    } else if (isLVD()) {       /* low voltage detect reset
                                 * It seems that the PIC32 libraries don't
                                 * actually support this, as reset.h has this
                                 * definition:
                                 * #define isLVD()							0
                                 */
        REPORT((REP_WARNING, "LVD reset"));

        return LOW_VOLTAGE_RESET;
    } else if (isMCLR()) {      /* master clear (reset) pin reset */
        mClearMCLRFlag();

        REPORT((REP_INFO, "MCLR reset"));

        return MASTER_CLEAR_RESET;
    } else if (isWDTTO()) {     /* watchdog timer reset */
        mClearWDTOFlag();

        REPORT((REP_ERROR, "WDT reset"));

        return WDT_RESET;
    } else if (mGetSWRFlag()) { /* software reset */
        mClearSWRFlag();

        REPORT((REP_WARNING, "SWR reset"));

        return SOFTWARE_RESET;
    } else if (mGetCMRFlag()) { /* config mismatch reset */
        mClearCMRFlag();

        REPORT((REP_CRITICAL, "CMR reset"));

        return CONFIG_MISMATCH_RESET;
    };

    return 0;
}

int32_t
init_leds(struct led *led0p, struct led *led1p)
{
    int32_t ret = 0;
    
    if (!led0p) {
        ret = -ENULPTR;
    } else {
        IF_ERR(led_new(led0p, NU32_LED0_PORT, NU32_LED0_PIN),
                REP_WARNING, "led_new") {
            ;   /* do nothing */
        } else {
            register_reporting_dev(&(led0p->erd), REP_ERROR);
        }
    }

    if (!led1p)
        ret = -ENULPTR;
    else
        REPORT_ON_ERR(led_new(led1p, NU32_LED1_PORT, NU32_LED1_PIN),
            REP_WARNING, "led_new");

    return ret;
}

int32_t
init_serial(struct serial *serp)
{
    BYTE delims[2] = {'\r', '\n'};

    if (!serp)
        return -ENULPTR;

    IF_ERR(serial_new(serp, SERIAL_MODULE, SERIAL_BAUD, NO_UART_INTERRUPT,
            INT_PRIORITY_DISABLED, 0, UART_DATA_SIZE_8_BITS, 0,
            UART_ENABLE|UART_TX|UART_RX, delims, sizeof(delims)),
            REP_WARNING,
            "serial_new failed") {
        ;   /* do nothing */
    } else {
        register_reporting_dev(&(serp->erd), REP_DEBUG);
    }

    return 0;
}

int32_t
init_nokia(struct nokia5110 *dp)
{
    if (!dp)
        return -ENULPTR;

    IF_ERR(nokia5110_new(dp, NOKIA_SPI_CHANNEL,
                                NOKIA_CS_PIN_LTR, NOKIA_CS_PIN_NUM,
                                NOKIA_RESET_PIN_LTR, NOKIA_RESET_PIN_NUM,
                                NOKIA_DC_PIN_LTR, NOKIA_DC_PIN_NUM),
                            REP_WARNING, "nokia5110_new") {
        ;   /* do nothing */
    } else {
        register_reporting_dev(&(dp->erd), REP_DEBUG);
    }

    return 0;
}

int32_t
init_can(struct can *canp)
{
    if (!canp)
        return -ENULPTR;

    IF_ERR(can_new_easy(canp, COMMON_CAN_MOD, 0, INT_PRIORITY_DISABLED),
            REP_WARNING, "can_new_easy") {
        ; /* do nothing */
    } else {
        canp->error_reporting_can_chn = COMMON_CAN_TX_CHN;
        canp->error_reporting_can_use_extended_id = STANDARD_ID;
        canp->error_reporting_can_std_id = ADDR_BMSTX(ERROR);
        canp->error_reporting_can_ext_id = 0;

        canp->op->addChannelTx(canp, COMMON_CAN_TX_CHN,
            CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_TX_RTR_DISABLED,
            CAN_HIGH_MEDIUM_PRIORITY, 0);

        canp->op->addChannelRx(canp, COMMON_CAN_RX_CHN,
                CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_RX_FULL_RECEIVE, 0);

        register_reporting_dev(&(canp->erd), REP_DEBUG);
    }

    return 0;
}

int32_t
init_ltcs(struct ltc6803 *ltcp)
{
    union BpsConfig cfg[LTC6803_COUNT];

    if (!ltcp)
        return -ENULPTR;

    memset(cfg, 0, sizeof(cfg));

    /* LTC6803 configuration: measurement only mode, 13ms minimum measurement time */
    cfg[0].cfgr0 = WDT | LVLPL | GPIO1 | GPIO2 | CDC_MSMTONLY;
    cfg[0].vov = convertOVLimit(OVER_VOLTAGE);
    cfg[0].vuv = convertUVLimit(UNDER_VOLTAGE);
    cfg[2] = cfg[1] = cfg[0];

    IF_ERR(ltc6803_new(ltcp, LTC6803_SPI_CHN, LTC6803_CS_PIN_LTR,
                    LTC6803_CS_PIN_NUM, LTC6803_COUNT, cfg),
                REP_CRITICAL, "ltc6803_new")
        return nu_errno;

    return 0;
}

int32_t
init_adcs(struct ad7685 *adp)
{
    if (!adp)
        return -ENULPTR;

        /* @TODO check the mode on this */
#warning Check the mode on this...
    IF_ERR(ad7685_new(adp, ADC_SPI_CHN, ADC_CS_PIN_LTR,
                      ADC_CS_PIN_NUM, 2, THREE_WIRE, NO_BUSY_INDICATOR),
            REP_CRITICAL, "ad7685_new")
        return nu_errno;

    return 0;
}

int32_t
init_ds18x20s(struct ds18x20 *dsp)
{
    if (!dsp)
        return -ENULPTR;

    IF_ERR(ds18x20_new(dsp, DS18X20_PIN_LTR, DS18X20_PIN_NUM,
                    PARASITIC_POWER_DISABLE),
                REP_CRITICAL, "ds18x20_new")
        return nu_errno;

    return 0;
}

int32_t
init_devices(struct nokia5110 *dp, struct ltc6803 *ltcp,
        struct ad7685 *adp,
        struct ds18x20 *dsp, struct can *canp,
        struct serial *serp,
        struct led *led0p, struct led *led1p)
{
    if (!dp || !ltcp || !adp || !dsp || !canp || !serp || !led0p || !led1p)
        REPORT_ERR((REP_WARNING, -ENULPTR, "init_devices passed NULL pointer"));

    REPORT_ON_ERR(init_leds(led0p, led1p),  REP_WARNING, "init_leds");

    REPORT_ON_ERR(init_serial(serp),        REP_WARNING, "init_serial");

    REPORT_ON_ERR(init_nokia(dp),           REP_WARNING, "init_nokia");

    REPORT_ON_ERR(init_can(canp),           REP_WARNING, "init_can");

    IF_ERR(init_ltcs(ltcp),                 REP_WARNING, "init_ltcs")
        TRIP_NOMOD(TRIP_OTHER);

    IF_ERR(init_adcs(adp),                  REP_WARNING, "init_adcs")
        TRIP_NOMOD(TRIP_OTHER);

    IF_ERR(init_ds18x20s(dsp),              REP_WARNING, "init_ds18x20s")
        TRIP_NOMOD(TRIP_OTHER);

    return 0;
}

void
nu_trip(struct can *canp, enum tripCode code, uint32_t module)
{
    union can_bms can_bms;

    can_bms.tx.trip.module = module;
    can_bms.tx.trip.tripCode = code;

    REPORT((REP_EMERGENCY, "tripping with code %d (%s); module %d",
        code, tripcodeStr[code], module));

    if (canp)
        canp->op->tx(canp, COMMON_CAN_TX_CHN, STANDARD_ID, ADDR_BMSTX(TRIP), 0,
                0, &(can_bms.tx.trip), sizeof(can_bms.tx.trip));

    while (1)
        ; /* do nothing */
}

int32_t
sendLastResetCode(struct can *self)
{
    union can_bms can_bms = {0};

    if (self == NULL)
        return -ENULPTR;

    can_bms.tx.last_reset.lastResetCode = getLastResetCause();
    REPORT_ON_ERR(self->op->tx(self, COMMON_CAN_TX_CHN, STANDARD_ID,
            ADDR_BMSTX(LAST_RESET), 0, 0, &(can_bms.tx.last_reset),
            sizeof(can_bms.tx.last_reset)), REP_WARNING,
            "failed to send last reset code");

    return 0;
}

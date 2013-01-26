#include "can.h"
#include "common_pragmas.h"
#include "nokia5110.h"
#include "pinctl.h"

static const CAN(ws_can,        CAN1);
static const CAN(common_can,    CAN2);
static const NOKIA5110(display,
        SPI_CHANNEL2, IOPORT_E, BIT_9, IOPORT_G, BIT_9, IOPORT_A, BIT_9);

/* analog in */
static const PIN(regen_pedal,       IOPORT_B, BIT_0);
static const PIN(accel_pedal,       IOPORT_B, BIT_1);
static const PIN(airgap_pot,        IOPORT_B, BIT_4);

/* digital in */
static const PIN(brake_pedal,       IOPORT_B, BIT_2);
static const PIN(headlight_switch,  IOPORT_B, BIT_3);
static const PIN(airgap_enable,     IOPORT_B, BIT_5);
static const PIN(regen_enable,      IOPORT_B, BIT_8);
static const PIN(reverse_switch,    IOPORT_B, BIT_9);

/* digital out */
static const PIN(lights_brake,      IOPORT_D, BIT_0);
static const PIN(lights_l,          IOPORT_D, BIT_1);
static const PIN(lights_r,          IOPORT_D, BIT_2);
static const PIN(headlights,        IOPORT_D, BIT_3);

#include <float.h>
#include <stdint.h>

#include "can_all.h"
#include "compiler.h"
#include "error_reporting.h"
#include "nokia5110.h"
#include "nu32.h"
#include "serial.h"
#include "ws20.h"

#include "common_pragmas.h"

enum CRUISE_STATE {
    CRUISE_DISABLED = 0,
    CRUISE_FORWARD,
    CRUISE_BACKWARD,
};

/**************************
 * configuration settings
 **************************/

static const uint32_t       SYS_CLK_HZ                  = 80000000;

/**********
 * pins
 */

static const SpiChannel     NOKIA_SPI_CHANNEL           = SPI_CHANNEL2;
static const IoPortId       NOKIA_DC_PIN_LTR            = IOPORT_E;
static const uint32_t       NOKIA_DC_PIN_NUM            = BIT_9;
static const IoPortId       NOKIA_CS_PIN_LTR            = IOPORT_G;
static const uint32_t       NOKIA_CS_PIN_NUM            = BIT_9;
static const IoPortId       NOKIA_RESET_PIN_LTR         = IOPORT_A;
static const uint32_t       NOKIA_RESET_PIN_NUM         = BIT_9;

/*********
 * peripheral config
 */

static const CAN_CHANNEL   WAVESCULPTOR_CAN_RX_CHN      = CAN_CHANNEL0;
static const CAN_CHANNEL   WAVESCULPTOR_CAN_TX_CHN      = CAN_CHANNEL1;
static const CAN_CHANNEL   COMMON_CAN_RX_CHN            = CAN_CHANNEL0;
static const CAN_CHANNEL   COMMON_CAN_TX_CHN            = CAN_CHANNEL1;
static const CAN_CHANNEL   COMMON_CAN_TX_ERR_CHN        = CAN_CHANNEL2;
static const float         ACCEL_PEDAL_OFFSET           = 0;
static const float         ACCEL_PEDAL_DIVIDER          = 1024;
static const float         ACCEL_PEDAL_THRESHOLD        = 0.05;
static const float         REGEN_PEDAL_OFFSET           = 0;
static const float         REGEN_PEDAL_DIVIDER          = 1024;
static const float         AIRGAP_OFFSET                = 0;
static const float         AIRGAP_DIVIDER               = 1024;

static const float         REGEN_AMOUNT                 = .2;

/*************
 * state
 *************/

static uint32_t     rawAccelPedalReading        = 0;
static uint32_t     rawRegenPedalReading        = 0;
static uint32_t     rawAirgapReading            = 0;
static float        accelPedalPercent           = 0;
static float        regenPedalPercent           = 0;
static float        airgapPercent               = 0;

static uint32_t     brakePedalEngaged           = 0;
static uint32_t     reverse                     = 0;
static uint32_t     headlightsOn                = 0;
static uint32_t     regenEnabled                = 0;
static uint32_t     airgapEnabled               = 0;
static uint32_t     hornOn                      = 0;
static uint32_t     leftLightOn                 = 0;
static uint32_t     rightLightOn                = 0;

static uint32_t     cruising                    = 0;

static ALWAYSINLINE void
setDigitalOut(IoPortId ltr, uint32_t num)
{
    PORTSetPinsDigitalOut(ltr, num);
    PORTClearBits(ltr, num);
}

static ALWAYSINLINE void
init_ios(void)
{    
    ClearWDT();

    CloseADC10();
    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF);
    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF|ADC_CH0_POS_SAMPLEA_AN0|ADC_CH0_NEG_SAMPLEB_NVREF|ADC_CH0_POS_SAMPLEB_AN1);
    PORTSetPinsAnalogIn(ACCEL_PEDAL_PIN_LTR,    ACCEL_PEDAL_PIN_NUM);
    PORTSetPinsAnalogIn(REGEN_PEDAL_PIN_LTR,    REGEN_PEDAL_PIN_NUM);
    PORTSetPinsAnalogIn(AIRGAP_POT_PIN_LTR,     AIRGAP_POT_PIN_NUM);
//    OpenADC10(ADC_MODULE_ON|ADC_FORMAT_INTG32|ADC_CLK_AUTO|ADC_AUTO_SAMPLING_ON,
//                ADC_VREF_AVDD_AVSS|ADC_OFFSET_CAL_DISABLE|ADC_SCAN_ON|ADC_SAMPLES_PER_INT_1|ADC_BUF_16|ADC_ALT_INPUT_OFF,
//                ADC_CONV_CLK_PB|ADC_CONV_CLK_3Tcy2|ADC_SAMPLE_TIME_10|ADC_CONV_CLK_PB,
//                SKIP_SCAN_ALL,
//                ACCEL_PEDAL_PIN_NUM|REGEN_PEDAL_PIN_NUM|AIRGAP_POT_PIN_NUM);
    /* define setup parameters for OpenADC10
     *             Turn module on | ouput in integer | trigger mode auto | enable autosample
     */
    #define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON

    /*              ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode */
    #define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON

    /*                use ADC internal clock | set sample time */
    #define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15

    /* do not assign channels to scan */
    #define PARAM4    SKIP_SCAN_ALL

    /* set AN4 and AN5 as analog inputs */
    #define PARAM5    ENABLE_AN0_ANA | ENABLE_AN1_ANA

    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 ); // configure ADC using parameter define above

    EnableADC10();

    while (!mAD1GetIntFlag())   /* get first conversion so there will be valid data */
        ; /* do nothing */

    PORTSetPinsDigitalIn(REVERSE_SWITCH_PIN_LTR,    REVERSE_SWITCH_PIN_NUM);
    PORTSetPinsDigitalIn(HEADLIGHT_SWITCH_PIN_LTR,  HEADLIGHT_SWITCH_PIN_NUM);
    PORTSetPinsDigitalIn(REGEN_ENABLE_PIN_LTR,      REGEN_ENABLE_PIN_NUM);
    PORTSetPinsDigitalIn(AIRGAP_ENABLE_PIN_LTR,     AIRGAP_ENABLE_PIN_NUM);
    PORTSetPinsDigitalIn(BRAKE_PEDAL_PIN_LTR,       BRAKE_PEDAL_PIN_NUM);

    setDigitalOut(LIGHTS_BRAKE_PIN_LTR, LIGHTS_BRAKE_PIN_NUM);
    setDigitalOut(LIGHTS_L_PIN_LTR,     LIGHTS_L_PIN_NUM);
    setDigitalOut(LIGHTS_R_PIN_LTR,     LIGHTS_R_PIN_NUM);
    setDigitalOut(HEADLIGHTS_PIN_LTR,   HEADLIGHTS_PIN_NUM);
    setDigitalOut(HORN_PIN_LTR,         HORN_PIN_NUM);
}

static ALWAYSINLINE int32_t
init_cans(void)
{
    int32_t err;

    ClearWDT();

    IF_NOERR(err = can_new_easy(commonCanp, COMMON_CAN_MOD, 0, INT_PRIORITY_DISABLED),
            REP_WARNING, "can_new_easy") {

        commonCanp->error_reporting_can_chn = COMMON_CAN_TX_ERR_CHN;
        commonCanp->error_reporting_can_use_extended_id = STANDARD_ID;
        commonCanp->error_reporting_can_std_id = ADDR_BMS_TX_ERROR;
        commonCanp->error_reporting_can_ext_id = 0;

        commonCanp->op->addChannelRx(commonCanp, COMMON_CAN_RX_CHN,
            32, CAN_RX_FULL_RECEIVE, 0);
//        commonCanp->op->addFilter(commonCanp, COMMON_CAN_RX_CHN, CAN_FILTER0,
//            CAN_SID, ADDR_DC_RX_BASE, CAN_FILTER_MASK0,
//            CAN_FILTER_MASK_IDE_TYPE, 0x7F0);

        commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_CHN,
            32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);
        commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_ERR_CHN,
            32, CAN_TX_RTR_DISABLED, CAN_LOWEST_PRIORITY, 0);

        register_reporting_dev(&(commonCanp->erd), REP_DEBUG);
    }

    /*
    can_new(ws20_canp, MPPT_CAN_MOD, 125E3, CAN_BIT_6TQ, CAN_BIT_4TQ,
            CAN_BIT_5TQ, AUTO_SET, THREE_TIMES, CAN_BIT_4TQ, 0,
            INT_PRIORITY_DISABLED, 0)
     */

    can_new_easy(ws20canp, WS_CAN_MOD, 0, INT_PRIORITY_DISABLED);
    ws20canp->op->addChannelTx(ws20canp, WAVESCULPTOR_CAN_TX_CHN,
            32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY, 0);
    
    return 0;
}

static ALWAYSINLINE int32_t
init_nokia(void)
{
    ClearWDT();

    IF_NOERR(nokia5110_new(dp,  NOKIA_SPI_CHANNEL,
                                NOKIA_CS_PIN_LTR, NOKIA_CS_PIN_NUM,
                                NOKIA_RESET_PIN_LTR, NOKIA_RESET_PIN_NUM,
                                NOKIA_DC_PIN_LTR, NOKIA_DC_PIN_NUM),
                REP_WARNING, "nokia5110_new")
        register_reporting_dev(&(dp->erd), REP_DEBUG);

    return 0;
}

static ALWAYSINLINE void
init_devices(void)
{
    ClearWDT();

    init_ios();
    init_cans();
    init_nokia();
}

static ALWAYSINLINE void
getAnalogIn(void)
{
    ClearWDT();

    rawAccelPedalReading    = ReadADC10(1);
    rawRegenPedalReading    = ReadADC10(REGEN_PEDAL_PIN_NUM);
    rawAirgapReading        = ReadADC10(AIRGAP_POT_PIN_NUM);

    accelPedalPercent = ((float)rawAccelPedalReading + ACCEL_PEDAL_OFFSET)/ACCEL_PEDAL_DIVIDER;
    if (accelPedalPercent < 0) {
        accelPedalPercent = 0;
        /* @TODO Some error or warning */
    } else if (accelPedalPercent > 1) {
        accelPedalPercent = 1;
        /* @TODO Some error or warning */
    }

    regenPedalPercent = ((float)rawRegenPedalReading + REGEN_PEDAL_OFFSET)/REGEN_PEDAL_DIVIDER;
    if (regenPedalPercent < 0) {
        regenPedalPercent = 0;
        /* @TODO Some error or warning */
    } else if (regenPedalPercent > 1) {
        regenPedalPercent = 1;
        /* @TODO Some error or warning */
    }

    airgapPercent = ((float)rawAirgapReading+AIRGAP_OFFSET)/AIRGAP_DIVIDER;
    if (airgapPercent < 0) {
        airgapPercent = 0;
    } else if (airgapPercent > 1) {
        airgapPercent = 1;
    }
}

static ALWAYSINLINE void
getDigitalIn(void)
{
    ClearWDT();

    reverse =
            PORTReadBits(REVERSE_SWITCH_PIN_LTR, REVERSE_SWITCH_PIN_NUM);
    headlightsOn =
            PORTReadBits(HEADLIGHT_SWITCH_PIN_LTR, HEADLIGHT_SWITCH_PIN_NUM);
    regenEnabled =
            PORTReadBits(REGEN_ENABLE_PIN_LTR, REGEN_ENABLE_PIN_NUM);
    airgapEnabled =
            PORTReadBits(AIRGAP_ENABLE_PIN_LTR, AIRGAP_ENABLE_PIN_NUM);
    brakePedalEngaged =
            PORTReadBits(BRAKE_PEDAL_PIN_LTR, BRAKE_PEDAL_PIN_NUM);
}

static ALWAYSINLINE void
getInput(void)
{
    ClearWDT();
    getAnalogIn();
    getDigitalIn();
}

static void
doDrive(void)
{
    static uint32_t lastCruiseChange = 0;
    struct can_ws20_rx_drive_cmd driveCmd = {0};

    ClearWDT();

    /* @TODO cruise */

    /* are we braking? */
    if (brakePedalEngaged) {
        if (regenEnabled) {
            driveCmd.motorCurrent = REGEN_AMOUNT;
            driveCmd.motorVelocity = 0;
        } else {
            driveCmd.motorCurrent = 0;
            driveCmd.motorVelocity = 0;
        }
        cruising = 0;
        lastCruiseChange = ReadCoreTimer();
    } else if (accelPedalPercent > ACCEL_PEDAL_THRESHOLD) {
        driveCmd.motorCurrent = accelPedalPercent;
        driveCmd.motorVelocity = 100;
    }

    if (reverse)
        driveCmd.motorVelocity *= -1;

    nu32_led0p->op->on(nu32_led0p);
    delay_ms(100);
    nu32_led0p->op->off(nu32_led0p);
    ws20canp->op->tx(ws20canp, WAVESCULPTOR_CAN_TX_CHN, STANDARD_ID,
            ADDR_WS20_RX_DRIVE_CMD, 0, 0, &driveCmd, sizeof(driveCmd));
}

static void
doCanRx(void)
{
    union can_dc_rx frame;
    uint32_t sid;
    uint32_t ui;

    CLEARWDT();

    for (ui = 0;
            ui < 32 && commonCanp->op->rx(commonCanp, COMMON_CAN_RX_CHN, &sid, &frame) > 0;
            ++ui) {
        dp->op->clear(dp);
        dp->op->gotoXY(dp, 0, 0);
        dp->op->printf(dp, "id %d=%X", sid, sid);
        switch(sid) {
        case ADDR_DC_RX_HORN:
            hornOn = (frame.horn.enabled > 0);
            break;
        case ADDR_DC_RX_SIGNALS:
            leftLightOn = frame.signals.l;
            rightLightOn = frame.signals.r;
            break;
        default:
            break;
        }
    }

    /*
    for (ui = 0;
            ui < 32 && ws20canp->op->rx(ws20canp, WS_CAN_))
     */
}

static ALWAYSINLINE void
doHorn(void)
{
    CLEARWDT();

    if (hornOn)
        PORTSetBits(HORN_PIN_LTR, HORN_PIN_NUM);
    else
        PORTClearBits(HORN_PIN_LTR, HORN_PIN_NUM);
}

static ALWAYSINLINE void
doLights(void)
{
    CLEARWDT();

    if (headlightsOn)
        PORTSetBits(HEADLIGHTS_PIN_LTR, HEADLIGHTS_PIN_NUM);
    else
        PORTClearBits(HEADLIGHTS_PIN_LTR, HEADLIGHTS_PIN_NUM);

    if (leftLightOn)
        PORTSetBits(LIGHTS_L_PIN_LTR, LIGHTS_L_PIN_NUM);
    else
        PORTClearBits(LIGHTS_L_PIN_LTR, LIGHTS_L_PIN_NUM);

    if (rightLightOn)
        PORTSetBits(LIGHTS_R_PIN_LTR, LIGHTS_R_PIN_NUM);
    else
        PORTClearBits(LIGHTS_R_PIN_LTR, LIGHTS_R_PIN_NUM);
}

int32_t
main(void)
{
    EnableWDT();
    nu32_init(SYS_CLK_HZ);
    nu32_init_leds();
    enableClearWdt();
    CLEARWDT();

    init_devices();
    WriteCoreTimer(0);

    while (1) {
        CLEARWDT();

        getInput();
        doCanRx();
        doDrive();
        doHorn();
        doLights();
        
        dp->op->clear(dp);
        dp->op->gotoXY(dp,0,0);
        dp->op->printf(dp, "%f", accelPedalPercent);
    }

//    while (1) {
//        ClearWDT();
//
//        getAnalogIn();
//
//
//
//
//        BOOL cruiseLockout = FALSE;
//
//        float cruiseCurrentPercent = 0, cruiseVelocityMperS = 0;
//        float currentPercent;
//        float currentVelocityMperS = 0;
//        float maxAccel = 0;
//
//        uint32_t lastBrakeTime = 0;
//        uint32_t canId = 0;
//        int32_t  len = 0;
//
//        enum CRUISE_STATE cruiseState = CRUISE_DISABLED;
//
//        ClearWDT();
//
//        if (ticksToSecs(ReadCoreTimer() - lastBrakeTime) > CRUISE_LOCKOUT_TIME_SECONDS)
//            cruiseLockout = FALSE;
//
//        union can_wavesculptor20_tx motorControllerMsg = {0};
//        len = ws20p->can.op->rx(&(ws20p->can), WAVESCULPTOR_CAN_RX_CHN,
//                                    &canId, &motorControllerMsg);
//        if (len >= 0) {
//            if (canId == ADDR_WAVESCULPTOR20_TX_MOTOR_VELOCITY_MSMT)
//                currentVelocityMperS = motorControllerMsg.velocity.vehVelocity;
//
//            /* forward any Wavesculptor messages to common CAN bus */
//            commonCanp->op->tx(commonCanp, COMMON_CAN_TX_CHN, STANDARD_ID,
//                                canId, 0, 0, &motorControllerMsg, len);
//        } else {
//            ;
//        }
//
//        union can_driver_controls_rx driverControlsMsg;
//        len = commonCanp->op->rx(commonCanp, COMMON_CAN_RX_CHN, &canId, &driverControlsMsg);
//
//        if (len >= 0) {
//            switch (canId) {
//            case ADDR_DRIVER_CONTROLS_RX_CRUISE:
//                if (!cruiseLockout && driverControlsMsg.cruise.cruiseEnabled)
//                    cruiseState = (cruiseVelocityMperS >= 0) ?
//                        CRUISE_FORWARD : CRUISE_BACKWARD;
//                else
//                    cruiseState = CRUISE_DISABLED;
//                break;
//            case ADDR_DRIVER_CONTROLS_RX_CRUISE_VELOCITY_CURRENT:
//                cruiseVelocityMperS = driverControlsMsg.cruise_velocity_current.velocity;
//                cruiseCurrentPercent = driverControlsMsg.cruise_velocity_current.current;
//                break;
//            }
//        } else {
//            ;
//        }
//
//        uint32_t    rawAccelPedalReading,   rawBrakePedalReading;
//        float       accelPedalPercent,      brakePedalPercent;
//        getAnalogIn(&rawAccelPedalReading, &accelPedalPercent,
//                            &rawBrakePedalReading, &brakePedalPercent);
//
//        ws20p->op->sendIdFrame(ws20p);
//
//        BOOL reverse = PORTReadBits(REVERSE_SWITCH_PIN_LTR, REVERSE_SWITCH_PIN_NUM);
//        BOOL regenEnabled = PORTReadBits(REGEN_SWITCH_PIN_LTR, REGEN_SWITCH_PIN_NUM);
//
//
//        /*Attempt to detangle the conditional mess below
//
//        if (brakePedalPercent > BRAKE_PEDAL_ACTIVATION_THRESHOLD_PERCENT)
//            currentPercent = brakePedalPercent;
//        else if (cruiseState != CRUISE_DISABLED)
//        {
//            currentPercent = MAX(cruiseCurrentPercent, accelPedalPercent);
//            velocity = cruiseVelocityMperS;
//        }
//        else
//            currentPercent = accelPedalPercent;
//
//        currentPercent = MIN(currentPercent, TRIP_THRESHOLD *
//                CAN.bms.tx.current_trip_pt.currentTripPt / );
//
//
//        */
//
//        /* Here lies aforementioned conditional mess.*/
//        if (brakePedalPercent > BRAKE_PEDAL_ACTIVATION_THRESHOLD_PERCENT) {
//            if (regenEnabled == TRUE)
//                ws20p->op->driveCmd(ws20p, brakePedalPercent, 0);
//            else
//                ws20p->op->driveCmd(ws20p, 0, 0);
//            cruiseState = CRUISE_DISABLED;
//            lastBrakeTime = ReadCoreTimer();
//            cruiseLockout = TRUE;
//            /* @TODO Send "brake engaged" message */
//        } else if ( ((cruiseState == CRUISE_FORWARD &&
//                        currentVelocityMperS < cruiseVelocityMperS)
//                    || (cruiseState == CRUISE_BACKWARD &&
//                        currentVelocityMperS > cruiseVelocityMperS))
//                    && accelPedalPercent < cruiseCurrentPercent) {
//            ws20p->op->driveCmd(ws20p, cruiseCurrentPercent, cruiseVelocityMperS);
//        } else if (reverse == TRUE || cruiseState == CRUISE_BACKWARD) {
//            ws20p->op->driveCmd(ws20p, accelPedalPercent, -FLT_MAX);
//        } else if (canFrame.bms.tx.current_draw.currentDraw >=
//                TRIP_THRESHOLD * canFrame.bms.tx.current_trip_pt.currentTripPt) {
//            /* The pedal is at or past the threshold (or was on the previous
//             * loop iteration). */
//            if ((maxAccel == 0) || (accelPedalPercent < maxAccel))
//                maxAccel = accelPedalPercent;
//            ws20p->op->driveCmd(ws20p, maxAccel, FLT_MAX);
//        } else if (canFrame.bms.tx.current_draw.currentDraw <
//                TRIP_THRESHOLD * canFrame.bms.tx.current_trip_pt.currentTripPt) {
//            /* The pedal is beneath the threshold. */
//            maxAccel = 0;
//            ws20p->op->driveCmd(ws20p, accelPedalPercent, FLT_MAX);
//        }
//
//    }

    return 0;
}

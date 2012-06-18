#include "main.h"

//#define MAX(x, y)   ((x) > (y) ? (x) : (y))
//#define MIN(x, y)   ((x) < (y) ? (x) : (y))


const float TRIP_THRESHOLD = 0.95;

int32_t
main(void)
{
    int32_t err = 0;
    struct can              commonCan, *commonCanp = &commonCan;
    struct wavesculptor20   ws20, *ws20p = &ws20;
    union can_anyFrame      CAN  = {.wavesculptor20.rx = ws20p->can.op->rx,
                                    .wavesculptor20.tx = ws20p->can.op->tx,
                                    .common.tx = commonCanp->op->tx};


    EnableWDT();
    nu32_init();
    ClearWDT();

    PORTSetPinsAnalogIn(ACCEL_PEDAL_PIN_LTR, ACCEL_PEDAL_PIN_NUM);
    PORTSetPinsAnalogIn(BRAKE_PEDAL_PIN_LTR, BRAKE_PEDAL_PIN_NUM);
    PORTSetPinsDigitalIn(REVERSE_SWITCH_PIN_LTR, REVERSE_SWITCH_PIN_NUM);

    if ((err = can_new_easy(commonCanp, COMMON_CAN_MOD, 0, INT_PRIORITY_DISABLED)) < 0)
        ;   /* @TODO error handling */

    commonCanp->op->addChannelTx(commonCanp, COMMON_CAN_TX_CHN,
            CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_TX_RTR_DISABLED,
            CAN_HIGH_MEDIUM_PRIORITY, 0);

    commonCanp->op->addChannelRx(commonCanp, COMMON_CAN_RX_CHN,
            CAN_TX_RX_MESSAGE_SIZE_BYTES, CAN_RX_DATA_ONLY, 0);

    if ((err = wavesculptor20_new_easy(ws20p, WAVESCULPTOR_CAN_MOD,
            WAVESCULPTOR_CAN_TX_CHN, WAVESCULPTOR_CAN_RX_CHN)) < 0)
        ;   /* @TODO error handling */
    


    while (1) {

        BOOL cruiseLockout = FALSE;

        float cruiseCurrentPercent = 0, cruiseVelocityMperS = 0;
        float currentPercent;
        float currentVelocityMperS = 0;
        float maxAccel = 0;

        uint32_t lastBrakeTime = 0;
        uint32_t canId = 0;
        int32_t  len = 0;

        enum CRUISE_STATE cruiseState = CRUISE_DISABLED;

        ClearWDT();

        if (ticksToSecs(ReadCoreTimer() - lastBrakeTime) > CRUISE_LOCKOUT_TIME_SECONDS)
            cruiseLockout = FALSE;


        

        union can_wavesculptor20_tx motorControllerMsg = {0};
        len = ws20p->can.op->rx(&(ws20p->can), WAVESCULPTOR_CAN_RX_CHN,
                                    &canId, &motorControllerMsg);
        if (len >= 0) {
            if (canId == ADDR_WAVESCULPTOR20_TX_MOTOR_VELOCITY_MSMT)
                currentVelocityMperS = motorControllerMsg.velocity.vehVelocity;

            /* forward any Wavesculptor messages to common CAN bus */
            commonCanp->op->tx(commonCanp, COMMON_CAN_TX_CHN, STANDARD_ID,
                                canId, 0, 0, &motorControllerMsg, len);
        } else {
            ;
        }

        union can_driver_controls_rx driverControlsMsg;
        len = commonCanp->op->rx(commonCanp, COMMON_CAN_RX_CHN, &canId, &driverControlsMsg);

        if (len >= 0) {
            switch (canId) {
            case ADDR_DRIVER_CONTROLS_RX_CRUISE:
                if (!cruiseLockout && driverControlsMsg.cruise.cruiseEnabled)
                    cruiseState = (cruiseVelocityMperS >= 0) ?
                        CRUISE_FORWARD : CRUISE_BACKWARD;
                else
                    cruiseState = CRUISE_DISABLED;
                break;
            case ADDR_DRIVER_CONTROLS_RX_CRUISE_VELOCITY_CURRENT:
                cruiseVelocityMperS = driverControlsMsg.cruise_velocity_current.velocity;
                cruiseCurrentPercent = driverControlsMsg.cruise_velocity_current.current;
                break;
            }
        } else {
            ;
        }

        uint32_t    rawAccelPedalReading,   rawBrakePedalReading;
        float       accelPedalPercent,      brakePedalPercent;
        getPedalReadings(&rawAccelPedalReading, &accelPedalPercent,
                            &rawBrakePedalReading, &brakePedalPercent);

        ws20p->op->sendIdFrame(ws20p);

        BOOL reverse = PORTReadBits(REVERSE_SWITCH_PIN_LTR, REVERSE_SWITCH_PIN_NUM);
        BOOL regenEnabled = PORTReadBits(REGEN_SWITCH_PIN_LTR, REGEN_SWITCH_PIN_NUM);


        /*Attempt to detangle the conditional mess below

        if (brakePedalPercent > BRAKE_PEDAL_ACTIVATION_THRESHOLD_PERCENT)
            currentPercent = brakePedalPercent;
        else if (cruiseState != CRUISE_DISABLED)
        {
            currentPercent = MAX(cruiseCurrentPercent, accelPedalPercent);
            velocity = cruiseVelocityMperS;
        }
        else
            currentPercent = accelPedalPercent;

        currentPercent = MIN(currentPercent, TRIP_THRESHOLD *
                CAN.bms.tx.current_trip_pt.currentTripPt / );


        */

        /* Here lies aforementioned conditional mess.*/
        if (brakePedalPercent > BRAKE_PEDAL_ACTIVATION_THRESHOLD_PERCENT) {
            if (regenEnabled == TRUE)
                ws20p->op->driveCmd(ws20p, brakePedalPercent, 0);
            else
                ws20p->op->driveCmd(ws20p, 0, 0);
            cruiseState = CRUISE_DISABLED;
            lastBrakeTime = ReadCoreTimer();
            cruiseLockout = TRUE;
            /* @TODO Send "brake engaged" message */
        } else if ( ((cruiseState == CRUISE_FORWARD &&
                        currentVelocityMperS < cruiseVelocityMperS)
                    || (cruiseState == CRUISE_BACKWARD &&
                        currentVelocityMperS > cruiseVelocityMperS))
                    && accelPedalPercent < cruiseCurrentPercent) {
            ws20p->op->driveCmd(ws20p, cruiseCurrentPercent, cruiseVelocityMperS);
        } else if (reverse == TRUE || cruiseState == CRUISE_BACKWARD) {
            ws20p->op->driveCmd(ws20p, accelPedalPercent, -FLT_MAX);
        } else if (CAN.bms.tx.current_draw.currentDraw >=
                TRIP_THRESHOLD * CAN.bms.tx.current_trip_pt.currentTripPt) {
            /* The pedal is at or past the threshold (or was on the previous
             * loop iteration). */
            if ((maxAccel == 0) || (accelPedalPercent < maxAccel))
                maxAccel = accelPedalPercent;
            ws20p->op->driveCmd(ws20p, maxAccel, FLT_MAX);
        } else if (CAN.bms.tx.current_draw.currentDraw <
                TRIP_THRESHOLD * CAN.bms.tx.current_trip_pt.currentTripPt) {
            /* The pedal is beneath the threshold. */
            maxAccel = 0;
            ws20p->op->driveCmd(ws20p, accelPedalPercent, FLT_MAX);
        }
        
    }

    return 0;
}

int32_t
getPedalReadings(uint32_t *rawAccelPedalReading, float *accelPedalPercent,
                    uint32_t *rawBrakePedalReading, float *brakePedalPercent)
{
    if (rawAccelPedalReading == NULL || accelPedalPercent == NULL ||
            rawBrakePedalReading == NULL || brakePedalPercent == NULL)
        return -ENULPTR;

    *rawAccelPedalReading = PORTReadBits(ACCEL_PEDAL_PIN_LTR, ACCEL_PEDAL_PIN_NUM);
    *rawBrakePedalReading = PORTReadBits(BRAKE_PEDAL_PIN_LTR, BRAKE_PEDAL_PIN_NUM);
    
    *accelPedalPercent = (*rawAccelPedalReading + ACCEL_PEDAL_OFFSET)*ACCEL_PEDAL_SCALER;
    if (*accelPedalPercent < 0) {
        *accelPedalPercent = 0;
        /* @TODO Some error or warning */
    } else if (*accelPedalPercent > 1) {
        *accelPedalPercent = 1;
        /* @TODO Some error or warning */
    }

    *brakePedalPercent = (*rawBrakePedalReading + BRAKE_PEDAL_OFFSET)*BRAKE_PEDAL_SCALER;
    if (*brakePedalPercent < 0) {
        *brakePedalPercent = 0;
        /* @TODO Some error or warning */
    } else if (*brakePedalPercent > 1) {
        *brakePedalPercent = 1;
        /* @TODO Some error or warning */
    }

    return 0;
}

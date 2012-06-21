#ifndef __NU_DRIVER_CONTROLS_MAIN_H
#define __NU_DRIVER_CONTROLS_MAIN_H

#include <float.h>
#include <stdint.h>
#include "error_reporting.h"
#include "nu32.h"
#include "serial.h"
#include "wavesculptor20.h"

#include "common_pragmas.h"

enum CRUISE_STATE {
    CRUISE_DISABLED = 0,
    CRUISE_FORWARD,
    CRUISE_BACKWARD,
};

const uint32_t      SYS_CLK_HZ                  = 80000000;

const CAN_MODULE    WAVESCULPTOR_CAN_MOD        = CAN1;
const CAN_CHANNEL   WAVESCULPTOR_CAN_TX_CHN     = CAN_CHANNEL0;
const CAN_CHANNEL   WAVESCULPTOR_CAN_RX_CHN     = CAN_CHANNEL1;

const CAN_MODULE    COMMON_CAN_MOD              = CAN2;
const CAN_CHANNEL   COMMON_CAN_TX_CHN           = CAN_CHANNEL0;
const CAN_CHANNEL   COMMON_CAN_RX_CHN           = CAN_CHANNEL1;

const IoPortId      REVERSE_SWITCH_PIN_LTR      = IOPORT_A;
const uint32_t      REVERSE_SWITCH_PIN_NUM      = 2;

const IoPortId      REGEN_SWITCH_PIN_LTR        = IOPORT_A;
const uint32_t      REGEN_SWITCH_PIN_NUM        = 3;

const IoPortId      ACCEL_PEDAL_PIN_LTR         = IOPORT_A;
const uint32_t      ACCEL_PEDAL_PIN_NUM         = 0;
const float         ACCEL_PEDAL_OFFSET          = 0;
const float         ACCEL_PEDAL_SCALER          = 1;

const IoPortId      BRAKE_PEDAL_PIN_LTR         = IOPORT_A;
const uint32_t      BRAKE_PEDAL_PIN_NUM         = 1;
const float         BRAKE_PEDAL_OFFSET          = 0;
const float         BRAKE_PEDAL_SCALER          = 1;
const float         BRAKE_PEDAL_ACTIVATION_THRESHOLD_PERCENT = 5;

/* how long after brake pedal is pressed before cruise can be re-enabled */
const double        CRUISE_LOCKOUT_TIME_SECONDS = 0.5;

#endif

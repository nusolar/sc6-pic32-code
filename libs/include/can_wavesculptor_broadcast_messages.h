#ifndef __NU_CAN_WAVESCULPTOR_BROADCAST_MESSAGES_H
#define __NU_CAN_WAVESCULPTOR_BROADCAST_MESSAGES_H

#ifdef __PIC32MX
#include <GenericTypeDefs.h>
#endif

#include <stdint.h>
#include "can_addresses.h"
#include "utility.h"

#define CAN_WS20TX(x)   can_wavesculptor20_tx_##x

/* interval: 1s */
struct CAN_WS20TX(ID) {
    /*   Type: Uint32
     *  Descr: "TRIa" stored as a string. tritiumId[0] = 'T', tritiumId[1] = 'R'... */
    char        tritiumId[4];

    /*   Type: Uint32
     *  Descr: Device serial number, allocated at manufacture. */
    uint32_t    serialNo;
};

/* interval: 200ms */
struct CAN_WS20TX(STATUS) {
    union {
        /* Flags indicate which control loop is limiting the output current of the motor controller */
        struct {
            unsigned    bridgePWM       :1;
            unsigned    motorCurrent    :1;
            unsigned    velocity        :1;
            unsigned    busCurrent      :1;
            unsigned    busVUpperLimit  :1;
            unsigned    busVLowerLimit  :1;
            unsigned    heatsinkTemp    :1;
            unsigned    reserved        :9;
        } __attribute__((__packed__));
        BYTE        limitFlagsByte[2];
        uint16_t    limitFlagsWord;
    } limitFlags;

    union {
        struct {
            unsigned    hwOC            :1; /* Hardware over current */
            unsigned    swOC            :1; /* Software over current */
            unsigned    dcBusOV         :1; /* DC Bus over voltage */
            unsigned    motorPosn       :1; /* Bad motor position hall sequence */
            unsigned    watchdog        :1; /* Watchdog caused last reset */
            unsigned    configRError    :1; /* Config read error */
            unsigned    fifteenVRUV     :1; /* 15V rail under voltage lock out occurred */
            unsigned    reserved        :9;
        } __attribute__ ((__packed__));
        BYTE            errorFlagsByte[2];
        uint16_t        errorFlagsWord;
    } errorFlags;

    /*   Type: Uint16
     *  Descr: The index of the motor currently being used. */
    uint16_t    activeMotor;

    unsigned    reserved :16 __attribute__ ((__packed__));
};

/* interval: 200ms */
struct CAN_WS20TX(BUS_VOLTAGE_CURRENT) {
    /*  Units: (V)oltage
     *  Descr: DC bus voltage at the controller */
    float busVoltage;

    /*  Units: (A)mperes
     *  Descr: Current drawn from the DC bus by the controller */
    float busCurrent;
};

/* interval: 200ms */
struct CAN_WS20TX(VELOCITY) {
    /* Units: rpm
     * Descr: Motor angular frequency */
     float motorVelocity;

    /*  Units: m/s
     *  Descr: Vehicle velocity */
     float vehVelocity;
};

/* interval: 200ms */
struct CAN_WS20TX(PHASE) {
    /* Units: (A)mperes_max
     * Descr: RMS current in motor Phase B */
    float phaseBCurrent;

    /* Units: (A)mperes_max
     * Descr: RMS current in motor Phase A */
    float phaseACurrent;
};

/* interval: 200ms */
struct CAN_WS20TX(VOLTAGE_VECTOR) {
    /* Units: (V)oltage
     * Descr: Imaginary component of applied non-rotating voltage vector to the motor */
    float voltageImag;

    /* Units: (V)oltage
     * Descr: Real component of applied non-rotating voltage vector to the motor */
    float voltageReal;
};

/* interval: 200ms */
struct CAN_WS20TX(CURRENT_VECTOR) {
    /* Units: (A)mperes
     * Descr: Imaginary component of applied non-rotating current vector to the motor.
     *        This current produces torque in the motor and should be in phase with
     *        the back-EMF of the motor. */
    float currentImag;

    /* Units: (A)mperes
     * Descr: Real component of applied non-rotating current vector to the motor.
     *        This vector represents the field current of the motor. */
    float currentReal;
};

/* interval: 200ms */
struct CAN_WS20TX(BACKEMF) {
    /* Units: (V)oltage
     * Descr: Peak of the phase to neutral motor voltage */
    float backEmfImag;

    /* Units: (V)oltage
     * Descr: By definition this value is always 0.
     */
    float backEmfReal;
};

/* interval: 1s */
struct CAN_WS20TX(15V_1PT65V) {
    /* Units: (V)oltage
     * Descr: Actual voltage level of the 1.65V analog reference */
    float onePtSixtyFiveVRef;

    /* Units: (V)oltage
     * Descr: Actual voltage level of the 15V power rail */
    float fifteenVPowerRail;
};

/* interval: 1s */
struct CAN_WS20TX(2PT5V_1PT2V) {
    /* Units: (V)oltage
     * Descr: Actual voltage level of the 1.2V DSP power rail */
    float onePtTwoVSupply;

    /* Units: (V)oltage
     * Descr: Actual voltage level of the 2.5V FPGA power rail */
    float twoPtFiveVSupply;
};

/* interval: 1s */
struct CAN_WS20TX(FANSPEED) {
    /* Units: % (0-1)
     * Descr: Drive voltage percent to cooling fan. If this value is above 0%, the fan should be spinning. */
    float fanDrive;

    /* Units: rpm
     * Descr: Cooling fan speed in revolutions per minute. */
    float fanRpm;
};

/* interval: 1s */
struct CAN_WS20TX(SINK_TEMP) {
    /* Units: deg. C
     * Descr: Internal temperature of the motor. */
    float motorTemp;

    /* Units: deg. C
     * Descr: Surface temperature of the controller heatsink. */
    float heatsinkTemp;
};

/* interval: 1s */
struct CAN_WS20TX(AIRIN_CPU_TEMP) {
    /* Units: deg. C
     * Descr: Temperature of the internal processor. */
    float processorTemp;

    /* Units: deg. C
     * Descr: Ambient temperature at the ventilation inlet of the controller. */
    float airInletTemp;
};

/* interval: 5s */
struct CAN_WS20TX(AIROUT_CAP_TEMP) {
    /* Units: deg. C
     * Descr: Ambient temperature of the internal bus capacitors. Unused by 20kW WaveSculptor. */
    float capacitorTemp;

    /* Units: deg. C
     * Descr: Ambient air temperature of the ventilation outlet of the controller. Unused in 20kW WaveSculptor. */
    float airOutTemp;
};

/* interval: 1s */
struct CAN_WS20TX(ODOM_BUS_AH) {
    /* Units: m
     * Descr: The distance the vehicle has travelled since reset. */
    float odom;

    /* Units: Ah (Ampere * hours)
     * Descr: The charge flow into the controller bus voltage from the time of reset. */
    float dcBusAmpHours;
};

union can_wavesculptor20_tx {
    struct CAN_WS20TX(ID)                   id;
    struct CAN_WS20TX(STATUS)               status;
    struct CAN_WS20TX(BUS_VOLTAGE_CURRENT)  bus_voltage_current;
    struct CAN_WS20TX(VELOCITY)             velocity;
    struct CAN_WS20TX(PHASE)                phase;
    struct CAN_WS20TX(VOLTAGE_VECTOR)       voltage_vector;
    struct CAN_WS20TX(CURRENT_VECTOR)       current_vector;
    struct CAN_WS20TX(BACKEMF)              backemf;
    struct CAN_WS20TX(15V_1PT65V)           fifteenV_1Pt65V;
    struct CAN_WS20TX(2PT5V_1PT2V)          twoPt5V1Pt2V;
    struct CAN_WS20TX(FANSPEED)             fanspeed;
    struct CAN_WS20TX(SINK_TEMP)            sink_temp;
    struct CAN_WS20TX(AIRIN_CPU_TEMP)       airin_cpu_temp;
    struct CAN_WS20TX(AIROUT_CAP_TEMP)      airout_cap_temp;
    struct CAN_WS20TX(ODOM_BUS_AH)          odom_bus_ah;
};

#endif

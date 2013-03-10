#ifndef NU_CAN_DC_H
#define NU_CAN_DC_H

#include "utility.h"

/* CAN for driver controls */

union can_dc {
    union can_dc_rx {
        struct can_dc_rx_horn {
            int64_t enabled;
        } horn;
        struct can_dc_rx_signals {
            uint32_t l;
            uint32_t r;
        } signals;
        struct can_dc_rx_reverseEnable {
            int64_t reverseEnabled;
        } reverseEnable;
        struct can_dc_rx_cruise {
            int64_t cruiseEnabled;
        } cruise;
        struct can_dc_rx_cruise_velocity_current {
            /*Units: m/s*/
            float velocity;
            /*Units: A*/
            float current;
        } cruise_velocity_current;
    } rx;
    union can_dc_tx {
        
    } tx;
};

STATIC_ASSERT(sizeof(union can_dc) <= 8, SIZE_MISMATCH);

#endif
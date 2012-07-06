#ifndef NU_CAN_STEERING_WHEEL_H
#define NU_CAN_STEERING_WHEEL_H

#include <stdint.h>

union can_steering_wheel {
    union can_steering_wheel_rx {
        struct can_steering_wheel_rx_lights {
            /* arranged left to right, top to bottom */
            unsigned    left            :1;
            unsigned    right           :1;
            unsigned    radio           :1;
            unsigned    yes             :1;
            unsigned    hazard          :1;
            unsigned    cruiseEnable    :1;
            unsigned    cruiseUp        :1;
            unsigned    maybe           :1;
            unsigned    no              :1;
            unsigned    horn            :1;
            unsigned    cruiseMode      :1;
            unsigned    cruiseDown      :1;
            unsigned    reserved        :20;
        } lights __attribute__((packed));
    } rx;
    union can_steering_wheel_tx {
        struct can_steering_wheel_tx_cruise {
            union {
                float   speedMperS;
                float   currentA;
            };
            enum cruiseMode {
                CRUISE_NONE,
                CRUISE_VELOCITY,
                CRUISE_CURRENT,
            } mode;
        } cruise;
        struct can_steering_wheel_tx_peripheral {
            unsigned    l           :1;
            unsigned    r           :1;
            unsigned    horn        :1;
            unsigned    reserved    :29;
        } peripheral __attribute__((packed));
        struct can_steering_wheel_tx_comm {
            enum comm {
                COMM_NONE,
                COMM_YES,
                COMM_NO,
                COMM_MAYBE,
            } comm;
            unsigned reserved :32;
        } comm;
    } tx;
};

#endif


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
            unsigned    cruise_en       :1;
            unsigned    cruise_up       :1;
            unsigned    maybe           :1;
            unsigned    no              :1;
            unsigned    horn            :1;
            unsigned    cruise_mode     :1;
            unsigned    cruise_down     :1;
            unsigned    reserved        :20;
        } lights __attribute__((packed));
    } rx;
    union can_steering_wheel_tx {
        struct can_steering_wheel_tx_buttons {
            unsigned    left            :1;
            unsigned    right           :1;
            unsigned    yes             :1;
            unsigned    no              :1;
            unsigned    maybe           :1;
            unsigned    hazard          :1;
            unsigned    horn            :1;
            unsigned    cruise_en       :1;
            unsigned    cruise_mode     :1;
            unsigned    cruise_up       :1;
            unsigned    cruise_down     :1;
            unsigned    reserved        :21;
        } buttons __attribute__((packed));
        struct can_steering_wheel_tx_lights {
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
    } tx;
};

STATIC_ASSERT(sizeof(struct can_steering_wheel_rx_lights) == 4, SIZE_MISMATCH);
STATIC_ASSERT(sizeof(struct can_steering_wheel_tx_buttons) == 4, SIZE_MISMATCH);
STATIC_ASSERT(sizeof(union can_steering_wheel) <= 8, SIZE_MISMATCH);

#endif


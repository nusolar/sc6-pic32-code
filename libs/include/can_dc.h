#ifndef NU_CAN_DC_H
#define NU_CAN_DC_H

/* CAN for driver controls */

union can_dc;
union can_dc_rx;

struct can_dc_rx_reverse {
    int64_t reverseEnabled;
};

struct can_dc_rx_cruise {
    int64_t cruiseEnabled;
};

struct can_dc_rx_cruise_velocity_current {
    /*Units: m/s*/
    float velocity;
    /*Units: A*/
    float current;
};

union can_dc_rx {
    struct can_dc_rx_reverse                    reverse;
    struct can_dc_rx_cruise                     cruise;
    struct can_dc_rx_cruise_velocity_current    cruise_velocity_current;
};

union can_dc {
    union can_dc_rx    rx;
};

#endif
#ifndef __NU_CAN_NU
#define __NU_CAN_NU

#ifndef __PIC32MX
typedef unsigned char BYTE;
#endif

#include "can_addresses.h"
#include "can_bms.h"
#include "can_wavesculptor20.h"
#include "utility.h"

#define CAN_DCRX(x)   can_driver_controls_rx_##x

struct CAN_DCRX(REVERSE) {
    int64_t reverseEnabled;
};

struct CAN_DCRX(CRUISE) {
    int64_t cruiseEnabled;
};

struct CAN_DCRX(CRUISE_VELOCITY_CURRENT) {
    /*Units: m/s*/
    float velocity;
    /*Units: A*/
    float current;
};

union can_driver_controls_rx {
    struct CAN_DCRX(REVERSE)                    reverse;
    struct CAN_DCRX(CRUISE)                     cruise;
    struct CAN_DCRX(CRUISE_VELOCITY_CURRENT)    cruise_velocity_current;
};

union can_driver_controls {
    union can_driver_controls_rx    rx;
};

#define CAN_COMMONTX(x)   can_common_tx_##x

struct CAN_COMMONTX(ERR_PRIORITY_NUM) {
    int32_t     priority;
    int32_t     errNum;
};

union can_common_tx {
    struct CAN_COMMONTX(ERR_PRIORITY_NUM)   errPriorityNum;
};

union can_common {
    union can_common_tx tx;
};

union can_anyFrame {
    union   can_wavesculptor20      wavesculptor20;
    union   can_driver_controls     driver_controls;
    union   can_bms                 bms;
    union   can_common              common;
};

#endif

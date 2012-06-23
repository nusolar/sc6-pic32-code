#ifndef __NU_CAN_BMS
#define __NU_CAN_BMS

#include <stdint.h>

#define CAN_BMSTX(x)    can_bms_tx_##x

struct CAN_BMSTX(LAST_RESET) {
    int32_t     lastResetCode;
    unsigned    reserved :32;
};

struct CAN_BMSTX(TRIP) {
    int32_t     tripCode;
    uint32_t    module;
};

struct CAN_BMSTX(CURRENT_DRAW) {
    float       currentDraw;
    unsigned    reserved    :32;
};

struct CAN_BMSTX(CURRENT_TRIP_PT) {
    float       currentTripPt;
    unsigned    reserved    :32;
};

struct CAN_BMSTX(HEARTBEAT) {
    unsigned    reserved    :32;
    unsigned    reserved2   :32;
};

union can_bms_tx {
    struct CAN_BMSTX(CURRENT_DRAW)      current_draw;
    struct CAN_BMSTX(CURRENT_TRIP_PT)   current_trip_pt;
    struct CAN_BMSTX(HEARTBEAT)         heartbeat;
    struct CAN_BMSTX(LAST_RESET)        last_reset;
    struct CAN_BMSTX(TRIP)              trip;
};

union can_bms {
    union can_bms_tx    tx;
};

#endif
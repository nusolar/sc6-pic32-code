#ifndef __NU_CAN_BMS
#define __NU_CAN_BMS

#include <stdint.h>

/** Trip codes that will be reported right before the car trips, and [hopefully]
 *  right after the car comes back up.
 */
#define TRIPCODES               \
    X(TRIP_NONE)                \
    X(TRIP_OTHER)               \
    X(TRIP_OW_BUS_FAILURE)      \
    X(TRIP_DS18X20_MISSING)     \
    X(TRIP_LTC_POST_FAILED)     \
    X(TRIP_ADC_FAILURE)         \
    X(TRIP_OVER_VOLTAGE)        \
    X(TRIP_UNDER_VOLTAGE)       \
    X(TRIP_OVER_CURRENT_DISCHRG)\
    X(TRIP_OVER_CURRENT_CHRG)   \
    X(TRIP_OVER_TEMP)           \
    X(TRIP_UNDER_TEMP)

#define X(x)    x,
enum tripCode {
    TRIPCODES
    NUM_TRIPCODES
};
#undef X

#define X(x)    #x,
static const char *tripcodeStr[NUM_TRIPCODES] = {
    TRIPCODES
};
#undef X

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
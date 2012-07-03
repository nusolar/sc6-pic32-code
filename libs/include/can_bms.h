#ifndef NU_CAN_BMS
#define NU_CAN_BMS

#include <stdint.h>

union can_bms {
    union can_bms_rx {
        struct can_bms_rx_trip {
            int32_t     tripCode;
            uint32_t    module;
        } trip;
    } rx;
    union can_bms_tx {
        struct can_bms_tx_last_reset {
            int32_t     lastResetCode;
            unsigned    reserved :32;
        } last_reset;
        
        struct can_bms_tx_last_trip {
            int32_t     tripCode;
            uint32_t    module;
        } last_trip;

        struct can_bms_tx_batt_bypass {
            uint32_t    module;
            unsigned    reserved :32;
        } batt_bypass;

        struct can_bms_tx_uptime {
            double      seconds;
        } uptime;

        struct can_bms_tx_trip {
            int32_t     tripCode;
            uint32_t    module;
        } trip;

        struct can_bms_tx_current {
            float       array;
            float       battery;
        } current;

        struct can_bms_tx_current_count_array {
            double      count;
        } current_count_array;
        
        struct can_bms_tx_current_count_battery {
            double      count;
        } current_count_battery;

        struct can_bms_tx_voltage {
            uint32_t    module;
            float       voltage;
        } voltage;
        
        struct can_bms_tx_owVoltage {
            uint32_t    module;
            float       owVoltage;
        } owVoltage;
        
        struct can_bms_tx_wh_batt {
            double      Wh;
        } wh_batt;

        struct can_bms_tx_temp {
            uint32_t    sensor;
            float       temp;
        } temp;

        struct can_bms_tx_trip_pt_current {
            float       low;
            float       high;
        } trip_pt_current;
        
        struct can_bms_tx_trip_pt_voltage {
            float       low;
            float       high;
        } trip_pt_voltage;
        
        struct can_bms_tx_trip_pt_temp {
            float       low;
            float       high;
        } trip_pt_temp;

        struct can_bms_tx_heartbeat {
            char        bmsStr[4];  /* "bms0" */
            unsigned    reserved2   :32;
        } heartbeat;
    } tx;
};

#endif

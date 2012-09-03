#ifndef NU_CAN_ADDRESSES_H
#define NU_CAN_ADDRESSES_H

/*
 * NP++ regex to cleanup old macros:
 * Find: ADDR_(.*?)RX\((.*?)\)
 * Replace with: ADDR_\1_RX_\2
 * Replace "RX" with "TX" as needed
 */

enum can_addresses {
    /* note: BMS RX filter mask currently limits RX addresses to 16 addresses:
     * ADDR_BMS_RX_BASE to ADDR_BMS_RX_BASE+15
     */
    ADDR_BMS_RX_BASE            = 0x200,
    ADDR_BMS_RX_TRIP            = ADDR_BMS_RX_BASE,
    ADDR_BMS_RX_RESET_CC_BATT,
    ADDR_BMS_RX_RESET_CC_ARRAY,
    ADDR_BMS_RX_RESET_CC_MPPT1,
    ADDR_BMS_RX_RESET_CC_MPPT2,
    ADDR_BMS_RX_RESET_CC_MPPT3,
    ADDR_BMS_RX_RESET_WH,
    ADDR_BMS_RX_RESET_ALL,
    ADDR_BMS_RX_MAX             = 0x20F,

    ADDR_BMS_TX_BASE            = 0x210,
    ADDR_BMS_TX_HEARTBEAT       = ADDR_BMS_TX_BASE,
    ADDR_BMS_TX_ERROR,  /* no struct */
    ADDR_BMS_TX_UPTIME,
    ADDR_BMS_TX_LAST_RESET,
    ADDR_BMS_TX_BATT_BYPASS,
    ADDR_BMS_TX_CURRENT,
    ADDR_BMS_TX_CC_ARRAY,
    ADDR_BMS_TX_CC_BATT,
    ADDR_BMS_TX_CC_MPPT1,
    ADDR_BMS_TX_CC_MPPT2,
    ADDR_BMS_TX_CC_MPPT3,
    ADDR_BMS_TX_WH_BATT,
    ADDR_BMS_TX_WH_MPPT1,
    ADDR_BMS_TX_WH_MPPT2,
    ADDR_BMS_TX_WH_MPPT3,
    ADDR_BMS_TX_VOLTAGE,
    ADDR_BMS_TX_OWVOLTAGE,
    ADDR_BMS_TX_TEMP,
    ADDR_BMS_TX_TRIP,
    ADDR_BMS_TX_LAST_TRIP,
    ADDR_BMS_TX_TRIP_PT_CURRENT,
    ADDR_BMS_TX_TRIP_PT_VOLTAGE,
    ADDR_BMS_TX_TRIP_PT_TEMP,
    
    ADDR_STEERING_WHEEL_RX_BASE     = 0x300,
    ADDR_STEERING_WHEEL_RX_LIGHTS   = ADDR_STEERING_WHEEL_RX_BASE,
    
    ADDR_STEERING_WHEEL_TX_BASE     = 0x310,
    ADDR_STEERING_WHEEL_TX_HEARTBEAT= ADDR_STEERING_WHEEL_TX_BASE,
    ADDR_STEERING_WHEEL_TX_ERROR,   /* no struct */
    ADDR_STEERING_WHEEL_TX_BUTTONS,
    ADDR_STEERING_WHEEL_TX_LIGHTS,

#warning "fixme"
    /* @TODO @HACK @FIXME */
    ADDR_WS20_RX_BASE               = 0x500,
    ADDR_WS20_RX_DRIVER_CONTROLS_ID = ADDR_WS20_RX_BASE,
    ADDR_WS20_RX_DRIVE_CMD,
    ADDR_WS20_RX_POWER_CMD,
    ADDR_WS20_RX_RESET_CMD,

    ADDR_WS20_TX_BASE                   = 0x500,
    ADDR_WS20_TX_MOTOR_ID               = ADDR_WS20_TX_BASE,
    ADDR_WS20_TX_MOTOR_STATUS_INFO,
    ADDR_WS20_TX_MOTOR_BUS_MSMT,
    ADDR_WS20_TX_MOTOR_VELOCITY_MSMT,
    ADDR_WS20_TX_MOTOR_PHASE_MSMT,
    ADDR_WS20_TX_MOTOR_VECTOR_MSMT,
    ADDR_WS20_TX_CURRENT_VECTOR_MSMT,
    ADDR_WS20_TX_BACKEMF_MSMT,
    ADDR_WS20_TX_15V_1PT65V_MSMT,
    ADDR_WS20_TX_2PT5V_1PT2V_MSMT,
    ADDR_WS20_TX_FANSPEED_MSMT,
    ADDR_WS20_TX_SINK_MOTOR_TEMP_MSMT,
    ADDR_WS20_TX_AIRIN_CPU_TEMP_MSMT,
    ADDR_WS20_TX_AIROUT_CAP_TEMP_MSMT,
    ADDR_WS20_TX_ODOM_BUS_AH_MSMT,
    
    ADDR_MPPT_RX_BASE           = 0b11100010000,    /* 0x710 */
    ADDR_MPPT1_RX_BASE,
    ADDR_MPPT2_RX_BASE,
    ADDR_MPPT3_RX_BASE,
    
    ADDR_MPPT_TX_BASE           = 0b11101110000,    /* 0x770 */
    ADDR_MPPT1_TX_BASE,
    ADDR_MPPT1_TX_FRAME         = ADDR_MPPT1_TX_BASE,
    ADDR_MPPT2_TX_BASE,
    ADDR_MPPT2_TX_FRAME         = ADDR_MPPT2_TX_BASE,
    ADDR_MPPT3_TX_BASE,
    ADDR_MPPT3_TX_FRAME         = ADDR_MPPT3_TX_BASE,

    /* driver controls */
    ADDR_DC_RX_BASE             = 0x110,
    ADDR_DC_RX_HORN             = ADDR_DC_RX_BASE,
    ADDR_DC_RX_SIGNALS,
    ADDR_DC_RX_CRUISE,
    ADDR_DC_RX_CRUISE_VELOCITY_CURRENT,

    ADDR_DC_TX_DRV_ID           = ADDR_WS20_RX_DRIVER_CONTROLS_ID,
    
    ADDR_MAX                    = 0x7FF,
};

#endif

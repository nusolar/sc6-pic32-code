#ifndef __NU_CAN_ADDRESSES_H
#define __NU_CAN_ADDRESSES_H

#define CAN_ADDR(x)             (ADDR_ ## x)

enum can_addresses {
#define ADDR_BMSRX(x)   ADDR_BMS_RX_##x
    ADDR_BMSRX(BASE)            = 0x200,
    ADDR_BMSRX(TRIP)            = ADDR_BMSRX(BASE),

#define ADDR_BMSTX(x)   ADDR_BMS_TX_##x
    ADDR_BMSTX(BASE)            = 0x210,
    ADDR_BMSTX(HEARTBEAT)       = ADDR_BMSTX(BASE),
    ADDR_BMSTX(CURRENT),
    ADDR_BMSTX(VOLTAGE),
    ADDR_BMSTX(TEMPERATURE),
    ADDR_BMSTX(TRIP),
    ADDR_BMSTX(ERROR),
    ADDR_BMSTX(LAST_RESET),
    ADDR_BMSTX(CURRENT_DRAW),
    ADDR_BMSTX(CURRENT_TRIP_PT),

#define ADDR_WS20RX(x)    ADDR_WAVESCULPTOR20_RX_##x
    ADDR_WS20RX(BASE)               = 0x400,
    ADDR_WS20RX(DRIVER_CONTROLS_ID) = ADDR_WS20RX(BASE),
    ADDR_WS20RX(DRIVE_CMD),
    ADDR_WS20RX(POWER_CMD),
    ADDR_WS20RX(RESET_CMD),

#define ADDR_WS20TX(x)  ADDR_WAVESCULPTOR20_TX_##x
    ADDR_WS20TX(BASE)                   = 0x500,
    ADDR_WS20TX(MOTOR_ID)               = ADDR_WS20TX(BASE),
    ADDR_WS20TX(MOTOR_STATUS_INFO),
    ADDR_WS20TX(MOTOR_BUS_MSMT),
    ADDR_WS20TX(MOTOR_VELOCITY_MSMT),
    ADDR_WS20TX(MOTOR_PHASE_MSMT),
    ADDR_WS20TX(MOTOR_VECTOR_MSMT),
    ADDR_WS20TX(CURRENT_VECTOR_MSMT),
    ADDR_WS20TX(BACKEMF_MSMT),
    ADDR_WS20TX(15V_1PT65V_MSMT),
    ADDR_WS20TX(2PT5V_1PT2V_MSMT),
    ADDR_WS20TX(FANSPEED_MSMT),
    ADDR_WS20TX(SINK_MOTOR_TEMP_MSMT),
    ADDR_WS20TX(AIRIN_CPU_TEMP_MSMT),
    ADDR_WS20TX(AIROUT_CAP_TEMP_MSMT),
    ADDR_WS20TX(ODOM_BUS_AH_MSMT),

#define ADDR_DCRX(x)    ADDR_DRIVER_CONTROLS_RX_##x
    ADDR_DCRX(BASE)             = 0x110,
    ADDR_DCRX(REVERSE)          = ADDR_DCRX(BASE),
    ADDR_DCRX(CRUISE),
    ADDR_DCRX(CRUISE_VELOCITY_CURRENT),

#define ADDR_DCTX(x)    ADDR_DRIVER_CONTROLS_TX_##x
    ADDR_DCTX(DRV_ID)           = ADDR_WS20RX(DRIVER_CONTROLS_ID),
};

#endif

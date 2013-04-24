#ifndef NU_CAN_WAVESCULPTOR_DRIVE_COMMANDS_H
#define NU_CAN_WAVESCULPTOR_DRIVE_COMMANDS_H 1

#ifdef __PIC32MX
#include <GenericTypeDefs.h>
#endif

#include <stdint.h>
#include "nu/can_addresses.h"
#include "nu/utility.h"

union can_ws20_rx;

/*
 * The following frame is not in the Tritium documentation, but can be
 * found in the Solar CAN Driver Controls source code (TRI63 source,
 * which can be found at http://www.tritium.com.au/products/legacy/).
 */
struct can_ws20_rx_driver_controls_id {
    /*   Type: Uint32
     *  Descr: "TRIb" stored as a string. drvId[0] = 'T', drvId[1] = 'R'... */
    char        drvId[4];

    /*   Type: Uint32
     *  Descr: Device serial number, allocated at manufacture. */
    uint32_t    serialNo;   /* just use 0x01 */
};

/*
 *  Must be received by the motor controller at least once every 250ms
 *  or else the controller will assume that communications have failed
 *  and will halt all motor control functions, placing the system into
 *  neutral.
 */
struct can_ws20_rx_drive_cmd {
    /*  Units: m/s
     *  Descr: Desired motor velocity set point in meters/second */
    float motorVelocity;

    /*  Units: % (0-1)
     *  Descr: Desired motor current set point as percentage of max current setting */
    float motorCurrent;
};

struct can_ws20_rx_power_cmd {
    unsigned reserved :32;

    /*  Units: % (0-1)
     *  Descr: Desired set point of current drawn from bus by controller as percentage of absolute bus current limit */
    float busCurrent;
};

/*
 *  Re-initialises all software modules in the DSP. Slightly
 *  different from pushing the reset button. Send a command
 *  from this address to reset the WaveSculptor.
 */
struct can_ws20_rx_reset_cmd {
    unsigned unused1 :32;
    unsigned unused2 :32;
};

union can_ws20_rx {
    struct can_ws20_rx_driver_controls_id   driver_controls_id;
    struct can_ws20_rx_drive_cmd            drive_cmd;
    struct can_ws20_rx_power_cmd            power_cmd;
    struct can_ws20_rx_reset_cmd            reset_cmd;
};

#endif

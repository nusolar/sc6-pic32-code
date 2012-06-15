#ifndef __NU_CAN_WAVESCULPTOR_DRIVE_COMMANDS_H
#define __NU_CAN_WAVESCULPTOR_DRIVE_COMMANDS_H

#ifdef __PIC32MX
#include <GenericTypeDefs.h>
#endif

#include <stdint.h>
#include "can_addresses.h"
#include "utility.h"

#define CAN_WS20RX(x)   can_wavesculptor20_rx_##x

/*
 * The following frame is not in the Tritium documentation, but can be
 * found in the Solar CAN Driver Controls source code (TRI63 source,
 * which can be found at http://www.tritium.com.au/products/legacy/).
 */
struct CAN_WS20RX(DRIVER_CONTROLS_ID) {
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
struct CAN_WS20RX(DRIVE_CMD) {
    /*  Units: m/s
     *  Descr: Desired motor velocity set point in meters/second */
    float motorVelocity;

    /*  Units: % (0-1)
     *  Descr: Desired motor current set point as percentage of max current setting */
    float motorCurrent;
};

struct CAN_WS20RX(POWER_CMD) {
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
struct CAN_WS20RX(RESET_CMD) {
    unsigned unused1 :32;
    unsigned unused2 :32;
};

union can_wavesculptor20_rx {
    struct CAN_WS20RX(DRIVER_CONTROLS_ID)   driver_controls_id;
    struct CAN_WS20RX(DRIVE_CMD)            drive_cmd;
    struct CAN_WS20RX(POWER_CMD)            power_cmd;
    struct CAN_WS20RX(RESET_CMD)            reset_cmd;
};

#endif

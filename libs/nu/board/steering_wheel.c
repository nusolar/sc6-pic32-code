#include "nu/board/steering_wheel.h"


s32
nu__SteeringWheel__main(void)
{
    while (1) {
        /* update button debounce values */
#define _PIN(name, ltr, num)    \
        btn_update(&name);
//        DIGITAL_IN_PINS
#undef _PIN
        /* nu_serial_printf(&display, "Speed %i", 25); */
        // check each btn_pressed
        // assemble CAN buttons packet
        // send
        // check each LED
        // assemble CAN LED packet
        // send
        // receive any CAN LED rx packets, implement
        // draw Status on Nokia
    }
    return 0;
}

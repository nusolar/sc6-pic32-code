#ifndef NU_NU32_H
#define NU_NU32_H 1

/**
 * @file
 * Common variables and utilities for Nick Marchuck's NU32 boards.
 *
 * Supports NU32v1 and NU32v2
 *
 * @sa http://hades.mech.northwestern.edu/index.php/NU32:_Introduction_to_the_PIC32
 */

#include "nu/compiler.h"
#include "nu/led.h"
#include "nu/param.h"
#include "nu/pinctl.h"
#include "nu/serial.h"

enum nu_nu32_version {
    NU_NU32_V1,
    NU_NU32_V2
};

/* recommendation: DON'T use the combination nu_nu32_led0 and nu_nu32_led2 */
DEPRECATED extern struct nu_led *nu_nu32_led0;
extern struct nu_led *nu_nu32_led1;
extern struct nu_led *nu_nu32_led2;
extern struct nu_pin *nu_nu32_switch;
#define nu_nu32_user nu_nu32_switch
/* recommendation: DON'T use combination nu_nu32_serial and nu_nu32_serial1 */
DEPRECATED extern struct nu_serial *nu_nu32_serial;
extern struct nu_serial *nu_nu32_serial1;
extern struct nu_serial *nu_nu32_serial2;

/**
 * @brief Sets up the clock, leds, and serial ports on the NU32 board
 */
void
nu_nu32_setup(enum nu_nu32_version version, unsigned long hz);

#endif

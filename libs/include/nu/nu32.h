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

#include "compiler.h"
#include "led.h"
#include "param.h"
#include "pinctl.h"
#include "serial.h"

#define NU_NU32_VERSIONS \
    NU_NU32_VERSION(NU_NU32_V1) \
    NU_NU32_VERSION(NU_NU32_V2)

enum nu_nu32_version {
#define NU_NU32_VERSION(v) v,
    NU_NU32_VERSIONS
#undef NU_NU32_VERSION
    NU_NUM_NU32_VERSIONS
};

extern enum nu_nu32_version nu_nu32_version;
extern const char *nu_nu32_versions[];

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

void
nu_nu32_setup_leds(void);

#endif

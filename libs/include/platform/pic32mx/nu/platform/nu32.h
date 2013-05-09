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
/* #include "nu/platform/nu32_pinctl.h" */
#include "nu/serial.h"

enum _nu_nu32_version {
    NU_NU32_V1,
    NU_NU32_V2
};

typedef enum _nu_nu32_version nu_nu32_version_t;

extern struct nu_pin *nu_nu32_switch;
#define nu_nu32_user nu_nu32_switch

void
nu_nu32_setup(nu_nu32_version_t version, nu_hz_t hz);

#endif

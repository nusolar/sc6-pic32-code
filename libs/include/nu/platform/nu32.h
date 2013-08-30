#ifndef NU_PLATFORM_NU32_H
#define NU_PLATFORM_NU32_H 1

/**
 * @file
 * Common variables and utilities for Nick Marchuck's NU32 boards.
 *
 * Supports NU32v1 and NU32v2
 *
 * @sa http://hades.mech.northwestern.edu/index.php/NU32:_Introduction_to_the_PIC32
 */

#include "nu/led.h"
#include "nu/param.h"
#include "nu/pinctl.h"
#include "nu/serial.h"
#include "nu/compiler.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32.h"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

enum _nu_nu32_version {
    NU_NU32_V1,
    NU_NU32_V2
};

typedef enum _nu_nu32_version nu_nu32_version_t;

extern struct nu_pin *nu_nu32_switch;
#define nu_nu32_user nu_nu32_switch

void
nu_nu32_setup(nu_nu32_version_t version, nu_hz_t hz);

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_NU32_H */

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

#include "nu/component/led.h"
#include "nu/peripheral/serial.h"
#include "nu/peripheral/pinctl.h"
#include "nu/param.h"
#include "nu/compiler.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32.h"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

enum __nu__Nu32__Version {
    NU_NU32_V1,
    NU_NU32_V2
};

typedef enum __nu__Nu32__Version nu__Nu32__Version_t;

extern struct nu__Pin *nu__Nu32__switch;
#define nu__Nu32__user nu__Nu32__switch

void
nu__Nu32__setup(nu__Nu32__Version_t version, nu__Param__Hz_t hz);

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_NU32_H */

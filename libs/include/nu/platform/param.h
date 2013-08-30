#ifndef NU_PLATFORM_PARAM_H
#define NU_PLATFORM_PARAM_H 1

#include "nu/types.h"
#include "nu/param.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/param.h!"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC

#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu_hz)

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

#include <xc.h>
#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu_hz/((u32)(1 << OSCCONbits.PBDIV)))

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_PARAM_H */

#ifndef NU_PARAM_H
#define NU_PARAM_H 1

#include "nu/platform.h"
#include "nu/types.h"

#ifdef __cplusplus
extern "C" {
#endif


#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/param.h!"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC

#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu__Param__Hz)

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

#include <xc.h>
#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu__Param__Hz/((u32)(1 << OSCCONbits.PBDIV)))

#endif /* NU_PLATFORM switch */


typedef u32 nu__Param__Hz_t;

extern nu__Param__Hz_t nu__Param__Hz;
#define NU_HZ   nu__Param__Hz
#define NU_MHZ  (NU_HZ/1000000UL)


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

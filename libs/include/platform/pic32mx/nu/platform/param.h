#ifndef NU_PIC32MX_PARAM_H
#define NU_PIC32MX_PARAM_H 1

#include "nu/types.h"
#include "nu/param.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <xc.h>

#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu_hz/((u32)(1 << OSCCONbits.PBDIV)))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#ifndef NU_PIC32MX_PARAM_H
#define NU_PIC32MX_PARAM_H 1

#include "nu/nu_types.h"

#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu_hz/((u32)(1 << OSCONbits.PBDIV)))

#endif


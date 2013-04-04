#ifndef NU_PARAM_H
#define NU_PARAM_H 1

#include "nu_types.h"

extern unsigned long nu_hz;

#define NU_DEFAULT_HZ (80000000UL)
#define NU_PBUS_FREQ_HZ (nu_hz/((u32)(1 << OSCCONbits.PBDIV)))

#endif

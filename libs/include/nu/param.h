#ifndef NU_PARAM_H
#define NU_PARAM_H 1

#include "nu/nu_types.h"
#include "platform/param.h"

extern u64 nu_hz;

#ifndef NU_DEFAULT_HZ
#define NU_DEFAULT_HZ (80000000UL)
#endif

#define NU_HZ   nu_hz
#define NU_MHZ  (NU_HZ/1000000UL)

#endif

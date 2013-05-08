#ifndef NU_PARAM_H
#define NU_PARAM_H 1

#include "nu/platform/param.h"
#include "nu/types.h"

typedef u32 nu_hz_t;

#ifndef NU_DEFAULT_HZ
# define NU_DEFAULT_HZ (80000000UL)
#endif

extern nu_hz_t nu_hz;
#define NU_HZ   nu_hz
#define NU_MHZ  (NU_HZ/1000000UL)

#endif

#ifndef NU_PARAM_H
#define NU_PARAM_H 1

#include "nu/types.h"
#include "nu/platform/param.h"

#ifndef NU_DEFAULT_HZ
# define NU_DEFAULT_HZ (80000000UL)
#endif

extern unsigned long nu_hz;

#define NU_HZ   nu_hz
#define NU_MHZ  (NU_HZ/1000000UL)

#endif

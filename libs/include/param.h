#ifndef NU_PARAM_H
#define NU_PARAM_H

/*
 * HZ       - clock speed in Hz
 * NU32_V   - version of the NU32 board. Can currently be either 1 or 2
 */

#ifndef HZ
# warning "Clock speed, HZ, not defined. Assuming 80MHz"
# define HZ (80000000UL)
#endif

#ifndef NU32_V
# warning "NU32_V not defined, assuming NU32_V = 2 (nu32v2)"
# define NU32_V 2
#endif

#endif

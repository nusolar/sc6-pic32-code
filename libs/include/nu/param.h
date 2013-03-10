#ifndef NU_PARAM_H
#define NU_PARAM_H

/**
 * @file
 * System setup parameters
 *
 * The following parameters should be specified by the user:
 *  - HZ, system clock speed in HZ. Defaults to 80MHz
 *  - NU32_V, the version of the NU32 board. Can currently be either 1 or 2
 */

#ifndef HZ
 #pragma message "Clock speed, HZ, not defined. Assuming 80MHz"
# define HZ (80000000UL)
#endif

#define PBUS_FREQ_HZ    (HZ/((u32)(1 << OSCCONbits.PBDIV)))

#ifndef NU32_V
 #pragma message "NU32_V not defined, assuming NU32_V = 2 (nu32v2)"
# define NU32_V 2
#endif

#endif

#ifndef __NU_ERRORCODES_H
#define __NU_ERRORCODES_H

#include <stdint.h>
#include "utility.h"

/* "The X Macro" technique from Dr. Dobb's
 * (http://www.drdobbs.com/blogs/cpp/228700289)
 * This is to avoid having to manually update the cstring array in
 * errorcodes.c
 */
#define ERRORS \
    X(ENONE)            /* no error (success) */                            \
    X(EOTHER)           /* unspecified error */                             \
    X(EINVALIDOP)       /* invalid operation */                             \
    X(EINVAL)           /* invalid argument */                              \
    X(ENULPTR)          /* null pointer (similar to EINVAL) */              \
    X(ETIMEOUT)         /* operation timed out */                           \
    X(ENODATA)          /* no data available */                             \
    X(ECRC)             /* CRC did not match expected CRC */                \
    X(ENODEV)           /* no device detected */                            \
    X(EEXCEEDSFLASHSIZ) /* exceeded set flash size on flash read/write */   \
    X(ETRUNCATED)       /* data/msg too long & truncated on operation */    \
    X(ETRIP)            /* car is tripping...this is currently unused
                         * and could potentially be removed */              \
    X(ELTC6803CFG)      /* LTC6803 cfg did not match expected */            \
    X(ELTC6803ADC)      /* LTC6803 ADC failure */                           \
    X(ELTC6803MUX)      /* LTC6803 MUX failure */                           \
    X(ELTC6803REF)      /* LTC6803 VREF failure */                          \
    X(EREPORTNOFREEDEVS)

#define X(a) a,
enum errors {
    ERRORS
    NUM_ERRORS
};
#undef X

const char *
getErrorName(int32_t err);

#endif

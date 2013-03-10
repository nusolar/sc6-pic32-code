#ifndef NU_ERRORCODES_H
#define NU_ERRORCODES_H

#include "nu_types.h"
#include "utility.h"

/* "The X Macro" technique from Dr. Dobb's
 * (http://www.drdobbs.com/blogs/cpp/228700289)
 * This is to avoid having to manually update the cstring array in
 * errorcodes.c
 */
#define ERRORS \
    ERROR(ENONE)            /* no error (success) */                            \
    ERROR(EOTHER)           /* unspecified error */                             \
    ERROR(EINVALIDOP)       /* invalid operation */                             \
    ERROR(EINVAL)           /* invalid argument */                              \
    ERROR(ENULPTR)          /* null pointer (similar to EINVAL) */              \
    ERROR(ETIMEOUT)         /* operation timed out */                           \
    ERROR(ENODATA)          /* no data available */                             \
    ERROR(ECRC)             /* CRC did not match expected CRC */                \
    ERROR(ENODEV)           /* no device detected */                            \
    ERROR(EEXCEEDSFLASHSIZ) /* exceeded set flash size on flash read/write */   \
    ERROR(ETRUNCATED)       /* data/msg too long & truncated on operation */    \
    ERROR(ETRIP)            /* car is tripping...this is currently unused
                             * and could potentially be removed */              \
    ERROR(ELTC6803CFG)      /* LTC6803 cfg did not match expected */            \
    ERROR(ELTC6803ADC)      /* LTC6803 ADC failure */                           \
    ERROR(ELTC6803MUX)      /* LTC6803 MUX failure */                           \
    ERROR(ELTC6803REF)      /* LTC6803 VREF failure */                          \
    ERROR(EREPORTNOFREEDEVS)

enum errors {
#define ERROR(x) x,
    ERRORS
#undef ERROR
    NUM_ERRORS
};

const char *
get_error_name(s32 err);

#endif

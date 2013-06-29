#ifndef NU_ERRORCODES_H
#define NU_ERRORCODES_H 1

#include "nu/types.h"

/* "The X Macro" technique from Dr. Dobb's
 * (http://www.drdobbs.com/blogs/cpp/228700289)
 * This is to avoid having to manually update the cstring array in
 * errorcodes.c
 */
#define NU_ERRORS(X) \
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

#define NU_ERROR_ENUM(x) x,
enum errors {
    NU_ERRORS(NU_ERROR_ENUM)
    NUM_ERRORS
};

#define NU_ERROR_NAMES(x) #x,

extern const char *nu_error_names[];

const char *
nu_error_get_name(s32 err);

#endif

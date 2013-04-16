#ifndef NU_ERRORCODES_H
#define NU_ERRORCODES_H 1

#include <stdint.h>

#include "utility/data.h"
#include <stdlib.h>

/* "The X Macro" technique from Dr. Dobb's
 * (http://www.drdobbs.com/blogs/cpp/228700289)
 * This is to avoid having to manually update the cstring array in
 * errorcodes.c
 */
#define NU_ERRORS \
    NU_ERROR(ENONE)            /* no error (success) */                            \
    NU_ERROR(EOTHER)           /* unspecified error */                             \
    NU_ERROR(EINVALIDOP)       /* invalid operation */                             \
    NU_ERROR(EINVAL)           /* invalid argument */                              \
    NU_ERROR(ENULPTR)          /* null pointer (similar to EINVAL) */              \
    NU_ERROR(ETIMEOUT)         /* operation timed out */                           \
    NU_ERROR(ENODATA)          /* no data available */                             \
    NU_ERROR(ECRC)             /* CRC did not match expected CRC */                \
    NU_ERROR(ENODEV)           /* no device detected */                            \
    NU_ERROR(EEXCEEDSFLASHSIZ) /* exceeded set flash size on flash read/write */   \
    NU_ERROR(ETRUNCATED)       /* data/msg too long & truncated on operation */    \
    NU_ERROR(ETRIP)            /* car is tripping...this is currently unused
                                * and could potentially be removed */              \
    NU_ERROR(ELTC6803CFG)      /* LTC6803 cfg did not match expected */            \
    NU_ERROR(ELTC6803ADC)      /* LTC6803 ADC failure */                           \
    NU_ERROR(ELTC6803MUX)      /* LTC6803 MUX failure */                           \
    NU_ERROR(ELTC6803REF)      /* LTC6803 VREF failure */                          \
    NU_ERROR(EREPORTNOFREEDEVS)

namespace nu {
	enum errors {
#define NU_ERROR(x) x,
		NU_ERRORS
#undef NU_ERROR
		NUM_ERRORS
	};
	
	static const char *error_names[] = {
#define NU_ERROR(x) #x,
		NU_ERRORS
#undef NU_ERROR
	};
	
	const char *error_get_name(int32_t err) {
		err = (uint16_t) abs(err);
		return error_names[err < NUM_ERRORS? err: EOTHER];
	}
}
#endif

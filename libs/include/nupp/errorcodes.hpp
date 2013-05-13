#ifndef NUPP_ERRORCODES_HPP
#define NUPP_ERRORCODES_HPP 1

#include "nu/compiler.h"

/* Library Conflicts */
#ifdef EOTHER
# undef EOTHER
#endif
#ifdef EINVAL
# undef EINVAL
#endif
#ifdef ENODATA
# undef ENODATA
#endif
#ifdef ENODEV
# undef ENODEV
#endif

/* "The X Macro" technique from Dr. Dobb's
 * (http://www.drdobbs.com/blogs/cpp/228700289)
 * This is to avoid having to manually update the cstring array in
 * errorcodes.c
 */
#define NU_ERRORS(X) \
	X(ENONE)			/* no error (success) */                            \
	X(EOTHER)			/* unspecified error */                             \
	X(EINVALIDOP)		/* invalid operation */                             \
	X(EINVAL)			/* invalid argument */                              \
	X(ENULPTR)			/* null pointer (similar to EINVAL) */              \
	X(ETIMEOUT)			/* operation timed out */                           \
	X(ENODATA)			/* no data available */                             \
	X(ECRC)				/* CRC did not match expected CRC */                \
	X(ENODEV)			/* no device detected */                            \
	X(EEXCEEDSFLASHSIZ)	/* exceeded set flash size on flash read/write */   \
	X(ETRUNCATED)		/* data/msg too long & truncated on operation */    \
	X(ETRIP)			/* car is tripping...this is currently unused
						 * and could potentially be removed */              \
	X(ELTC6803CFG)		/* LTC6803 cfg did not match expected */            \
	X(ELTC6803ADC)		/* LTC6803 ADC failure */                           \
	X(ELTC6803MUX)		/* LTC6803 MUX failure */                           \
	X(ELTC6803REF)		/* LTC6803 VREF failure */                          \
	X(EREPORTNOFREEDEVS)

#define NU_ERROR_ENUM(x) x,
#define NU_ERROR_NAMES(x) #x,

namespace nu {
	namespace error {
		/** Standard error codes, which index error_names[] below */
		enum errors {
			NU_ERRORS(NU_ERROR_ENUM)
			NUM_ERRORS
		};

		/** Array of the string representations of the standard error codes */
		extern const char *names[];

		/** A wrapper to get (without failure) an error name */
		static ALWAYSINLINE const char *get_name(errors err) {
			return names[err < NUM_ERRORS ? err: EOTHER];
		}
	}
}
#endif

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

extern "C" {
#include "nu/errorcodes.h"
}

namespace nu {
	namespace error {
		/** Standard error codes, which index error_names[] below */
		enum errors {
			NU_ERRORS(NU_ERROR_ENUM)
			NUM_ERRORS
		};

		/** Array of the string representations of the standard error codes */
		static const char **names = nu_error_names;

		/** A wrapper to get (without failure) an error name */
		static ALWAYSINLINE const char *get_name(errors err) {
			return names[err < NUM_ERRORS ? err: EOTHER];
		}
	}
}
#endif

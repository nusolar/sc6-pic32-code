#ifndef NUPP_PARAM_HPP
#define NUPP_PARAM_HPP 1

#include "nu/compiler.h"
#include "nu/param.h"
#include "nupp/platform/param.hpp"

#include <cstdint>

namespace nu {
	/**
	 * System parameters, like the clock frequency and peripheral bus frequency.
	 */
	namespace param {
		static ALWAYSINLINE reg_t& Hz()		{ return nu_hz; }
		static ALWAYSINLINE reg_t MHz()	{ return NU_MHZ; }

		/** Set clock frequency, in MHz */
		static ALWAYSINLINE reg_t& MHz(reg_t mhz) { return Hz() = mhz*1000000; }

		static ALWAYSINLINE reg_t default_hz() { return NU_DEFAULT_HZ; }
	}
}

#endif

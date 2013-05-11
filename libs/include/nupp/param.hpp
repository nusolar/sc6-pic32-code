#ifndef NUPP_PARAM_HPP
#define NUPP_PARAM_HPP 1

#include "nu/compiler.h"
#include "nupp/platform/param.hpp"
extern "C" {
#include "nu/param.h"
}

#include <cstdint>

namespace nu {
	/**
	 * System parameters, like the clock frequency and peripheral bus frequency.
	 */
	namespace param {
		static ALWAYSINLINE typeof(nu_hz)& Hz()		{ return nu_hz; }
		static ALWAYSINLINE typeof(NU_MHZ) MHz()	{ return NU_MHZ; }

		/** Set clock frequency, in MHz */
		static ALWAYSINLINE typeof(nu_hz)&
		MHz(typeof(NU_MHZ) mhz) { return Hz() = mhz*1000000; }

		static ALWAYSINLINE typeof(NU_DEFAULT_HZ) default_hz() { return NU_DEFAULT_HZ; }
	}
}

#endif

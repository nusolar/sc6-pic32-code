#ifndef NUXX_PARAM_HPP
#define NUXX_PARAM_HPP 1

#include "nuxx/platform/param.hpp"
#include "nu/compiler.h"
#include "nu/param.h"

#include <cstdint>

namespace nu {
	/**
	 * System parameters, like the clock frequency and peripheral bus frequency.
	 */
	namespace param {
		static ALWAYSINLINE reg_t& Hz()		{ return nu__Param__Hz; }
		static ALWAYSINLINE reg_t  MHz()	{ return NU_MHZ; }

		static ALWAYSINLINE reg_t  default_hz() { return NU_DEFAULT_HZ; }

		static INLINE reg_t timer_hz() {return param::Hz()/param::timer_ticks_per_cycle();}
	}
}

#endif

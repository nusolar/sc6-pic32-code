#ifndef NUXX_PARAM_HPP
#define NUXX_PARAM_HPP 1

#include "nu/compiler.h"
#include "nu/param.h"
#include "nu/platform.h"
#include <cstdint>


#if NU_PLATFORM==NU_PLATFORM_GENERIC /* Generic param code - i.e, nothing*/
namespace nu {
	namespace param {
	}
}
#elif NU_PLATFORM==NU_PLATFORM_PIC32MX /* PIC32MX-specific Parameter code */
namespace nu {
	namespace param {
		/**
		 * Get Hz specific to Peripherial Bus if one exists. Otherwise get
		 * system clock Hz.
		 * @todo: Distinguish DEFAULT_PBUS_HZ from PBUS_HZ.
		 */
		static INLINE typeof(NU_PBUS_FREQ_HZ) pbus_hz() {return NU_PBUS_FREQ_HZ;}
		static PURE INLINE reg_t timer_ticks_per_cycle() {return 2;}
	}
}
#endif /* PLATFORM specific code */


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

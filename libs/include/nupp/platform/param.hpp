#ifndef NUPP_PLATFORM_PARAM_HPP
#define NUPP_PLATFORM_PARAM_HPP 1

#include "nu/platform/param.h"
#include "nu/compiler.h"
#include "nu/platform.h"

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
		 * @todo: Distinguish DEFAULT_PBUS_HZ from PBUS_HZ, initialize in nu32.h
		 */
		static INLINE typeof(NU_PBUS_FREQ_HZ) pbus_hz() {return NU_PBUS_FREQ_HZ;}
		static PURE INLINE reg_t timer_ticks_per_cycle() {return 2;}
	}
}

#endif /* PLATFORM code */

#endif /* NUPP_PLATFORM_PARAM_HPP */


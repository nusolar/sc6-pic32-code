#ifndef NUPP_PLATFORM_PARAM_HPP
#define NUPP_PLATFORM_PARAM_HPP 1

#include "nu/compiler.h"
extern "C" {
#include "nu/platform/param.h"
}

namespace nu {
	namespace param {
		/**
		 * Get Hz specific to Peripherial Bus if one exists. Otherwise get
		 * system clock Hz.
		 * @todo: Distinguish DEFAULT_PBUS_HZ from PBUS_HZ, initialize in nu32.h
		 */
		static ALWAYSINLINE typeof(NU_PBUS_FREQ_HZ) pbus_hz() {return NU_PBUS_FREQ_HZ;}
	}
}

#endif


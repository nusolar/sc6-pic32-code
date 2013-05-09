#ifndef NUPP_PLATFORM_PARAM_HPP
#define NUPP_PLATFORM_PARAM_HPP 1

#include "nu/compiler.h"
extern "C" {
#include "nu/platform/param.h"
}

namespace nu {
	namespace param {
		static ALWAYSINLINE typeof(NU_PBUS_FREQ_HZ) pbus_hz() {return NU_PBUS_FREQ_HZ;}
	}
}

#endif


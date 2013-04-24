#ifndef NUPP_PARAM_HPP
#define NUPP_PARAM_HPP 1

#include <cstdint>

namespace nu {
	/**
	 * Operating hardware-specific parameters.
	 */
	struct Param {
		static uint64_t nu_hz; // FUCK MPLAB
		
		#define NU_DEFAULT_HZ (80000000UL)
		#define NU_PBUS_FREQ_HZ (Param::nu_hz/((uint32_t)(1 << OSCCONbits.PBDIV)))
		#define HZ (80000000UL)
	};
}

#endif

#ifndef NUPP_PLATFORM_PARAM_HPP
#define NUPP_PLATFORM_PARAM_HPP 1

extern "C" {
#include "nu/compiler.h"
#include "nu/platform/param.h"
}

namespace nu { namespace platform {
    static ALWAYSINLINE typeof(NU_PBUS_FREQ_HZ)
    pbus_freq_hz() { return NU_PBUS_FREQ_HZ; }
}}

#endif

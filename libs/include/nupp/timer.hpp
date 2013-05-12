#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nupp/param.hpp"
#include "nu/compiler.h"
#include <cstdint>

#include <xc.h>
#include <plib.h>

namespace nu {
	namespace timer {
		/** Ticks happen every other clockcycle */
		static ALWAYSINLINE uint32_t ticks() {return ReadCoreTimer();}
		static ALWAYSINLINE void reset() {WriteCoreTimer(0);}

		static ALWAYSINLINE uint64_t  s_to_ticks(uint64_t s)  {return s *(uint64_t)param::Hz() /2;}
		static ALWAYSINLINE uint64_t ms_to_ticks(uint64_t ms) {return ms*(uint64_t)param::Hz() /2000;}
		static ALWAYSINLINE uint64_t us_to_ticks(uint64_t us) {return us*(uint64_t)param::MHz()/2;}
		static ALWAYSINLINE uint64_t ns_to_ticks(uint64_t ns) {return ns*(uint64_t)param::MHz()/2000;}

		static ALWAYSINLINE uint32_t  s() { return ticks()*2	/param::Hz(); }
		static ALWAYSINLINE uint32_t ms() { return ticks()*2000 /param::Hz(); }
		static ALWAYSINLINE uint32_t us() { return ticks()*2	/param::MHz(); }
		static ALWAYSINLINE uint32_t ns() { return ticks()*2000	/param::MHz(); }

		static ALWAYSINLINE void delay_ticks(const uint32_t t) {
			const uint32_t start = ticks();
			while (ticks() - start < (t))
				Nop();   // do nothing
		}

		static ALWAYSINLINE void delay_s (uint64_t s)  {delay_ticks((uint32_t)s_to_ticks(s));}
		static ALWAYSINLINE void delay_ms(uint64_t ms) {delay_ticks((uint32_t)ms_to_ticks(ms));}
		static ALWAYSINLINE void delay_us(uint64_t us) {delay_ticks((uint32_t)us_to_ticks(us));}
		static ALWAYSINLINE void delay_ns(uint64_t ns) {delay_ticks((uint32_t)ns_to_ticks(ns));}
	}
}
#endif

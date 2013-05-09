#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nupp/param.hpp"
#include "nu/compiler.h"
#include <cstdint>

#include <xc.h>
#include <plib.h>

namespace nu {
	namespace timer {
		static ALWAYSINLINE void reset() {WriteCoreTimer(0);}
		static ALWAYSINLINE uint32_t ticks() {return ReadCoreTimer();}

		static ALWAYSINLINE float s_to_ticks (float s)  { return s *(float)param::Hz() /2; }
		static ALWAYSINLINE float ms_to_ticks(float ms) { return ms*(float)param::Hz() /2000; }
		static ALWAYSINLINE float us_to_ticks(float us) { return us*(float)param::MHz()/2; }
		static ALWAYSINLINE float ns_to_ticks(float ns) { return ns*(float)param::MHz()/2000; }

		static ALWAYSINLINE uint32_t s(void)  { return ticks()*2	/param::Hz(); }
		static ALWAYSINLINE uint32_t ms(void) { return ticks()*2000 /param::Hz(); }
		static ALWAYSINLINE uint32_t us(void) { return ticks()*2	/param::MHz(); }
		static ALWAYSINLINE uint32_t ns(void) { return ticks()*2000	/param::MHz(); }

		static ALWAYSINLINE void delay_ticks(uint32_t t) {
			uint32_t start = ticks();
			while (ticks() - start < (t))
				Nop();   /* do nothing */
		}
		
		static ALWAYSINLINE void delay_s(float s) {delay_ticks((uint32_t)s_to_ticks(s));}
		static ALWAYSINLINE void delay_ms(float ms) {delay_ticks((uint32_t)ms_to_ticks(ms));}
		static ALWAYSINLINE void delay_us(float us) {delay_ticks((uint32_t)us_to_ticks(us));}
		static ALWAYSINLINE void delay_ns(float ns) {delay_ticks((uint32_t)ns_to_ticks(ns));}
	}
}
#endif

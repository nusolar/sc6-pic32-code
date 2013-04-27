#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nupp/param.hpp"
#include "nu/compiler.h"
#include <stdint.h>

#include "nu/utility.h"
#include <xc.h>
#include <plib.h>

namespace nu {
	//namespace timer {
#define timer_reset()   WriteCoreTimer(0)

		static ALWAYSINLINE float s_to_ticks (float s)  { return s *param::Hz() /2; }
		static ALWAYSINLINE float ms_to_ticks(float ms) { return ms*param::Hz() /2000; }
		static ALWAYSINLINE float us_to_ticks(float us) { return us*param::MHz()/2; }
		static ALWAYSINLINE float ns_to_ticks(float ns) { return ns*param::MHz()/2000; }

#define timer_ticks ReadCoreTimer
		static ALWAYSINLINE uint32_t timer_s(void)  { return timer_ticks()*2	/param::Hz(); }
		static ALWAYSINLINE uint32_t timer_ms(void) { return timer_ticks()*2000 /param::Hz(); }
		static ALWAYSINLINE uint32_t timer_us(void) { return timer_ticks()*2	/param::MHz(); }
		static ALWAYSINLINE uint32_t timer_ns(void) { return timer_ticks()*2000	/param::MHz(); }

		static ALWAYSINLINE void delay_ticks(uint32_t t) {
			uint32_t start = timer_ticks();
			while (timer_ticks() - start < (t/20))
				Nop();   /* do nothing */
		}

#define delay_s(s)      delay_ticks(s_to_ticks(s))
#define delay_ms(ms)    delay_ticks(ms_to_ticks(ms))
#define delay_us(us)    delay_ticks(us_to_ticks(us))
#define delay_ns(ns)    delay_ticks(ns_to_ticks(ns))
	//}
}
#endif

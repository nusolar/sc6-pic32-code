#ifndef NU_TIMER_H
#define NU_TIMER_H 1

#include "nu/param.h"
#include "nu/utility.h"
#include "nu/types.h"
#include "nu/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

	#include <peripheral/timer.h>

	#define nu__Timer__reset() WriteCoreTimer(0)
	#define nu__Timer__ticks() ReadCoreTimer()

	typedef u32 tick_t;
	typedef u32 hz_t;

	/* undefined functions to get compile-time errors */
	extern tick_t _nu__Timer__s_to_ticks_value_out_of_range(void);
	extern tick_t _nu__Timer__ms_to_ticks_value_out_of_range(void);
	extern tick_t _nu__Timer__us_to_ticks_value_out_of_range(void);
	extern tick_t _nu__Timer__ns_to_ticks_value_out_of_range(void);
	extern void _nu__Timer__delay_ticks_value_out_of_range(void);

	/* divide by 2
	 * max input of ~53 */
	static ALWAYSINLINE tick_t
	_nu__Timer__s_to_ticks(hz_t s)   { return (s*NU_HZ)>>1; }
	#define _const_nu__Timer__s_to_ticks(s) ((s*NU_HZ)>>1)

	/* divide by 2000
	 * max input of ~5300 */
	static ALWAYSINLINE tick_t
	_nu__Timer__ms_to_ticks(hz_t ms) { return (ms*(NU_HZ/100))/20; }
	#define _const_nu__Timer__ms_to_ticks(ms)   ((ms*(NU_HZ/100))/20)

	/* divide by 2000000
	 * max input of ~5300 */
	static ALWAYSINLINE tick_t
	_nu__Timer__us_to_ticks(hz_t us) { return (us*(NU_HZ/100))/20000; }
	#define _const_nu__Timer__us_to_ticks(us)   ((us*(NU_HZ/100))/20000)

	/* divide by 2000000000
	 * max input of ~5300 */
	static ALWAYSINLINE tick_t
	_nu__Timer__ns_to_ticks(hz_t ns) { return (ns*(NU_HZ/100))/20000000; }
	#define _const_nu__Timer__ns_to_ticks(ns)   ((ns*(NU_HZ/100))/20000000)

	static ALWAYSINLINE tick_t nu__Timer__s(void)   { return (nu__Timer__ticks()<<1)/NU_HZ; }
	static ALWAYSINLINE tick_t nu__Timer__ms(void)  { return (nu__Timer__ticks()*2000)/NU_HZ; }
	static ALWAYSINLINE tick_t nu__Timer__us(void)  { return (nu__Timer__ticks()<<1)/NU_MHZ; }
	static ALWAYSINLINE tick_t nu__Timer__ns(void)  { return (nu__Timer__ticks()*2000)/NU_MHZ; }

	static ALWAYSINLINE void
	_nu__Timer__delay_ticks(tick_t t)
	{
		tick_t start = nu__Timer__ticks();
		while (nu__Timer__ticks() - start < t)
			NU_NOP();
	}

	/* limited to 20s */
	#define nu__Timer__s_to_ticks(s) \
		(__builtin_constant_p(s) ? \
			((s) > 20 ? _nu__Timer__s_to_ticks_value_out_of_range() : _const_nu__Timer__s_to_ticks(s)) : \
			_nu__Timer__s_to_ticks(s))

	/* limited to 2000ms */
	#define nu__Timer__ms_to_ticks(ms) \
		(__builtin_constant_p(ms) ? \
			((ms) > 2000 ? _nu__Timer__ms_to_ticks_value_out_of_range() : _const_nu__Timer__ms_to_ticks(ms)) : \
			_nu__Timer__ms_to_ticks(ms))

	/* limited to 2000us */
	#define nu__Timer__us_to_ticks(us) \
		(__builtin_constant_p(us) ? \
			((us) > 2000 ? _nu__Timer__us_to_ticks_value_out_of_range() : _const_nu__Timer__us_to_ticks(us)) : \
			_nu__Timer__us_to_ticks(us))

	/* limited to 2000ns */
	#define nu__Timer__ns_to_ticks(ns) \
		(__builtin_constant_p(ns) ? \
			((ns) > 2000 ? _nu__Timer__ns_to_ticks_value_out_of_range() : _const_nu__Timer__ns_to_ticks(ns)) : \
			_nu__Timer__ns_to_ticks(ns))

	#define nu__Timer__delay_ticks(ticks) \
		(__builtin_constant_p(ticks) && (ticks) > __MAX(tick_t) ? \
			_nu__Timer__delay_ticks_value_out_of_range() : _nu__Timer__delay_ticks(ticks))

	#define nu__Timer__delay_s(s)      nu__Timer__delay_ticks(nu__Timer__s_to_ticks(s))
	#define nu__Timer__delay_ms(ms)    nu__Timer__delay_ticks(nu__Timer__ms_to_ticks(ms))
	#define nu__Timer__delay_us(us)    nu__Timer__delay_ticks(nu__Timer__us_to_ticks(us))
	#define nu__Timer__delay_ns(ns)    nu__Timer__delay_ticks(nu__Timer__ns_to_ticks(ns))

	extern uint32_t _nu__Timer__carry;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

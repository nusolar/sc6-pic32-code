#ifndef NU_TIMER_H
#define NU_TIMER_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include "nu/param.h"
#include "nu/utility.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <peripheral/timer.h>

#define nu_timer_reset() WriteCoreTimer(0)
#define nu_timer_ticks() ReadCoreTimer()

typedef typeof(nu_timer_ticks()) tick_t;
typedef typeof(NU_HZ) hz_t;

/* undefined functions to get compile-time errors */
extern tick_t _nu_s_to_ticks_value_out_of_range(void);
extern tick_t _nu_ms_to_ticks_value_out_of_range(void);
extern tick_t _nu_us_to_ticks_value_out_of_range(void);
extern tick_t _nu_ns_to_ticks_value_out_of_range(void);
extern void _nu_delay_ticks_value_out_of_range(void);

/* divide by 2
 * max input of ~53 */
static ALWAYSINLINE tick_t _nu_s_to_ticks(hz_t s)   { return (s*NU_HZ)>>1; }
#define _const_nu_s_to_ticks(s) ((s*NU_HZ)>>1)

/* divide by 2000
 * max input of ~5300 */
static ALWAYSINLINE tick_t _nu_ms_to_ticks(hz_t ms) { return (ms*(NU_HZ/100))/20; }
#define _const_nu_ms_to_ticks(ms)   ((ms*(NU_HZ/100))/20)

/* divide by 2000000
 * max input of ~5300 */
static ALWAYSINLINE tick_t _nu_us_to_ticks(hz_t us) { return (us*(NU_HZ/100))/20000; }
#define _const_nu_us_to_ticks(us)   ((us*(NU_HZ/100))/20000)

/* divide by 2000000000
 * max input of ~5300 */
static ALWAYSINLINE tick_t _nu_ns_to_ticks(hz_t ns) { return (ns*(NU_HZ/100))/20000000; }
#define _const_nu_ns_to_ticks(ns)   ((ns*(NU_HZ/100))/20000000)

static ALWAYSINLINE tick_t nu_timer_s(void)   { return (nu_timer_ticks()<<1)/NU_HZ; }
static ALWAYSINLINE tick_t nu_timer_ms(void)  { return (nu_timer_ticks()*2000)/NU_HZ; }
static ALWAYSINLINE tick_t nu_timer_us(void)  { return (nu_timer_ticks()<<1)/NU_MHZ; }
static ALWAYSINLINE tick_t nu_timer_ns(void)  { return (nu_timer_ticks()*2000)/NU_MHZ; }

static ALWAYSINLINE void
_nu_delay_ticks(tick_t t)
{
    tick_t start = nu_timer_ticks();
    while (nu_timer_ticks() - start < t)
        NU_NOP();
}

/* limited to 20s */
#define nu_s_to_ticks(s) \
    (__builtin_constant_p(s) ? \
        ((s) > 20 ? _nu_s_to_ticks_value_out_of_range() : _const_nu_s_to_ticks(s)) : \
        _nu_s_to_ticks(s))

/* limited to 2000ms */
#define nu_ms_to_ticks(ms) \
    (__builtin_constant_p(ms) ? \
        ((ms) > 2000 ? _nu_ms_to_ticks_value_out_of_range() : _const_nu_ms_to_ticks(ms)) : \
        _nu_ms_to_ticks(ms))

/* limited to 2000us */
#define nu_us_to_ticks(us) \
    (__builtin_constant_p(us) ? \
        ((us) > 2000 ? _nu_us_to_ticks_value_out_of_range() : _const_nu_us_to_ticks(us)) : \
        _nu_us_to_ticks(us))

/* limited to 2000ns */
#define nu_ns_to_ticks(ns) \
    (__builtin_constant_p(ns) ? \
        ((ns) > 2000 ? _nu_ns_to_ticks_value_out_of_range() : _const_nu_ns_to_ticks(ns)) : \
        _nu_ns_to_ticks(ns))

#define nu_delay_ticks(ticks) \
    (__builtin_constant_p(ticks) && (ticks) > __MAX(tick_t) ? \
        _nu_delay_ticks_value_out_of_range() : _nu_delay_ticks(ticks))

#define nu_delay_s(s)      nu_delay_ticks(nu_s_to_ticks(s))
#define nu_delay_ms(ms)    nu_delay_ticks(nu_ms_to_ticks(ms))
#define nu_delay_us(us)    nu_delay_ticks(nu_us_to_ticks(us))
#define nu_delay_ns(ns)    nu_delay_ticks(nu_ns_to_ticks(ns))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

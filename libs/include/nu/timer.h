#ifndef NU_TIMER_H
#define NU_TIMER_H

#include <peripheral/timer.h>
#include "compiler.h"
#include "nu_types.h"
#include "param.h"
#include "utility.h"

STATIC_ASSERT(HZ > 1000000, SYS_CLK_HZ_TOO_LOW);
#define MHZ (HZ/1000000)

#define timer_reset()   WriteCoreTimer(0)

static ALWAYSINLINE u32 s_to_ticks(u32 s)   { return (s*HZ)>>1; }
static ALWAYSINLINE u32 ms_to_ticks(u32 ms) { return (ms*HZ)/2000; }
static ALWAYSINLINE u32 us_to_ticks(u32 us) { return (us*MHZ)>>1; }
static ALWAYSINLINE u32 ns_to_ticks(u32 ns) { return (ns*MHZ)/2000; }

#define timer_ticks ReadCoreTimer
static ALWAYSINLINE u32 timer_s(void)   { return (timer_ticks()<<1)/HZ; }
static ALWAYSINLINE u32 timer_ms(void)  { return (timer_ticks()*2000)/HZ; }
static ALWAYSINLINE u32 timer_us(void)  { return (timer_ticks()<<1)/MHZ; }
static ALWAYSINLINE u32 timer_ns(void)  { return (timer_ticks()*2000)/MHZ; }

static ALWAYSINLINE void
delay_ticks(u32 t)
{
    u32 start = timer_ticks();
    while (timer_ticks() - start < t)
        Nop();   /* do nothing */
}

#define delay_s(s)      delay_ticks(s_to_ticks(s))
#define delay_ms(ms)    delay_ticks(ms_to_ticks(ms))
#define delay_us(us)    delay_ticks(us_to_ticks(us))
#define delay_ns(ns)    delay_ticks(ns_to_ticks(ns))

#endif

#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nupp/param.hpp"
#include "nupp/compiler.hpp"
#include <stdint.h>

#include "nupp/utility.hpp"
#include <xc.h>
#include <plib.h>

//STATIC_ASSERT(NU_HZ > 1000000, SYS_CLK_HZ_TOO_LOW);
#define MHZ (NU_HZ/1000000)

#define timer_reset()   WriteCoreTimer(0)

static ALWAYSINLINE uint32_t s_to_ticks(uint32_t s)   { return (s*NU_HZ)>>1; }
static ALWAYSINLINE uint32_t ms_to_ticks(uint32_t ms) { return (ms*NU_HZ)/2000; }
static ALWAYSINLINE uint32_t us_to_ticks(uint32_t us) { return (us*MHZ)>>1; }
static ALWAYSINLINE uint32_t ns_to_ticks(uint32_t ns) { return (ns*MHZ)/2000; }

#define timer_ticks ReadCoreTimer
static ALWAYSINLINE uint32_t timer_s(void)   { return (timer_ticks()<<1)/NU_HZ; }
static ALWAYSINLINE uint32_t timer_ms(void)  { return (timer_ticks()*2000)/NU_HZ; }
static ALWAYSINLINE uint32_t timer_us(void)  { return (timer_ticks()<<1)/MHZ; }
static ALWAYSINLINE uint32_t timer_ns(void)  { return (timer_ticks()*2000)/MHZ; }

static ALWAYSINLINE void
delay_ticks(uint32_t t)
{
    uint32_t start = timer_ticks();
    while (timer_ticks() - start < t)
        Nop();   /* do nothing */
}

#define delay_s(s)      delay_ticks(s_to_ticks(s))
#define delay_ms(ms)    delay_ticks(ms_to_ticks(ms))
#define delay_us(us)    delay_ticks(us_to_ticks(us))
#define delay_ns(ns)    delay_ticks(ns_to_ticks(ns))

#endif

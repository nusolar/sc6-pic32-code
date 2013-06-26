#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nu/compiler.h"
#include "nupp/param.hpp"
extern "C" {
#include "nu/timer.h"
}

namespace nu {
	namespace timer {

		/** Ticks happen every other clockcycle */
		static ALWAYSINLINE void reset() {nu_timer_reset();}

		template <u64 s>
		static ALWAYSINLINE u64  s_to_ticks()		{return nu_s_to_ticks(s);}
		static ALWAYSINLINE u64  s_to_ticks(u64 s)	{return nu_s_to_ticks(s);}
		template <u64 ms>
		static ALWAYSINLINE u64 ms_to_ticks()		{return nu_ms_to_ticks(ms);}
		static ALWAYSINLINE u64 ms_to_ticks(u64 ms)	{return nu_ms_to_ticks(ms);}
		template <u64 us>
		static ALWAYSINLINE u64 us_to_ticks()		{return nu_us_to_ticks(us);}
		static ALWAYSINLINE u64 us_to_ticks(u64 us)	{return nu_us_to_ticks(us);}
		template <u64 ns>
		static ALWAYSINLINE u64 ns_to_ticks()		{return nu_ns_to_ticks(ns);}
		static ALWAYSINLINE u64 ns_to_ticks(u64 ns)	{return nu_ns_to_ticks(ns);}

		static ALWAYSINLINE u64 s ()	{return (int64_t)(nu_timer_ticks()<<1)/NU_HZ;}
		static ALWAYSINLINE u64 ms()	{return (int64_t)nu_timer_ticks()*2000/NU_HZ;}
		static ALWAYSINLINE u64 us()	{return (int64_t)(nu_timer_ticks()<<1)/NU_MHZ;}
		static ALWAYSINLINE u64 ns()	{return (int64_t)(nu_timer_ticks()*2000)/NU_MHZ;}
		static ALWAYSINLINE u64 ticks()	{return nu_timer_ticks();}

		template <u64 ticks>
		static ALWAYSINLINE void delay_ticks()			{nu_delay_ticks(ticks);}
		static ALWAYSINLINE void delay_ticks(u64 ticks)	{nu_delay_ticks(ticks);}
		template <u64 s>
		static ALWAYSINLINE void delay_s ()			{delay_ticks((tick_t)(s*param::Hz()/2));}
		static ALWAYSINLINE void delay_s (u64 s)	{delay_ticks((tick_t)(s*param::Hz()/2));}
		template <u64 ms>
		static ALWAYSINLINE void delay_ms()			{delay_ticks((tick_t)(ms*param::Hz()/2000));}
		static ALWAYSINLINE void delay_ms(u64 ms)	{delay_ticks((tick_t)(ms*param::Hz()/2000));}
		template <u64 us>
		static ALWAYSINLINE void delay_us()			{delay_ticks((tick_t)(us*param::MHz()/2));}
		static ALWAYSINLINE void delay_us(u64 us)	{delay_ticks((tick_t)(us*param::MHz()/2));}
		template <u64 ns>
		static ALWAYSINLINE void delay_ns()			{delay_ticks((tick_t)(ns*param::MHz()/2000));}
		static ALWAYSINLINE void delay_ns(u64 ns)	{delay_ticks((tick_t)(ns*param::MHz()/2000));}

	}
}

#endif

#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nu/compiler.h"
extern "C" {
#include "nu/timer.h"
}

namespace nu {
	namespace timer {

		/** Ticks happen every other clockcycle */
		static ALWAYSINLINE void reset() {nu_timer_reset();}

		template <u32 s>
		static ALWAYSINLINE u32  s_to_ticks()		{return nu_s_to_ticks(s);}
		static ALWAYSINLINE u32  s_to_ticks(u32 s)	{return nu_s_to_ticks(s);}
		template <u32 ms>
		static ALWAYSINLINE u32 ms_to_ticks()		{return nu_ms_to_ticks(ms);}
		static ALWAYSINLINE u32 ms_to_ticks(u32 ms)	{return nu_ms_to_ticks(ms);}
		template <u32 us>
		static ALWAYSINLINE u32 us_to_ticks()		{return nu_us_to_ticks(us);}
		static ALWAYSINLINE u32 us_to_ticks(u32 us)	{return nu_us_to_ticks(us);}
		template <u32 ns>
		static ALWAYSINLINE u32 ns_to_ticks()		{return nu_ns_to_ticks(ns);}
		static ALWAYSINLINE u32 ns_to_ticks(u32 ns)	{return nu_ns_to_ticks(ns);}

		static ALWAYSINLINE u32 s ()	{return nu_timer_s();}
		static ALWAYSINLINE u64 ms()	{return (int64_t)nu_timer_ticks()*2000/NU_HZ;}// return nu_timer_ms();}
		static ALWAYSINLINE u32 us()	{return nu_timer_us();}
		static ALWAYSINLINE u32 ns()	{return nu_timer_ns();}
		static ALWAYSINLINE u32 ticks()	{return nu_timer_ticks();}

		template <u32 s>
		static ALWAYSINLINE void delay_s ()			{nu_delay_s(s);}
		static ALWAYSINLINE void delay_s (u32 s)	{nu_delay_s(s);}
		template <u32 ms>
		static ALWAYSINLINE void delay_ms()			{nu_delay_ms(ms);}
		static ALWAYSINLINE void delay_ms(u32 ms)	{nu_delay_ms(ms);}
		template <u32 us>
		static ALWAYSINLINE void delay_us()			{nu_delay_us(us);}
		static ALWAYSINLINE void delay_us(u32 us)	{nu_delay_us(us);}
		template <u32 ns>
		static ALWAYSINLINE void delay_ns()			{nu_delay_ns(ns);}
		static ALWAYSINLINE void delay_ns(u32 ns)	{nu_delay_ns(ns);}
		template <u32 ticks>
		static ALWAYSINLINE void delay_ticks()			{nu_delay_ticks(ticks);}
		static ALWAYSINLINE void delay_ticks(u32 ticks)	{nu_delay_ticks(ticks);}

	}
}

#endif

#ifndef NUPP_TIMER_HPP
#define NUPP_TIMER_HPP 1

#include "nupp/param.hpp"
#include "nu/timer.h"
#include "nu/compiler.h"

namespace nu {
	namespace timer {
		static INLINE void reset() {nu_timer_reset();}

		static INLINE u64  s_to_ticks(u64 s)	{return nu_s_to_ticks((hz_t)s);}
		static INLINE u64 ms_to_ticks(u64 ms)	{return nu_ms_to_ticks((hz_t)ms);}
		static INLINE u64 us_to_ticks(u64 us)	{return nu_us_to_ticks((hz_t)us);}
		static INLINE u64 ns_to_ticks(u64 ns)	{return nu_ns_to_ticks((hz_t)ns);}

		/** Ticks happen every other clockcycle */
		static INLINE u64 s ()	{return (((int64_t)nu_timer_ticks())<<1)/NU_HZ;}
		static INLINE u64 ms()	{return (int64_t)nu_timer_ticks()*2000/NU_HZ;}
		static INLINE u64 us()	{return (((int64_t)nu_timer_ticks())<<1)/NU_MHZ;}
		static INLINE u64 ns()	{return (int64_t)nu_timer_ticks()*2000/NU_MHZ;}
		static INLINE u64 ticks()	{return nu_timer_ticks();}

		static INLINE void delay_ticks(tick_t ticks)	{nu_delay_ticks(ticks);}
		static INLINE void delay_s (u64 s)	{delay_ticks((tick_t)(s*param::Hz()/2));}
		static INLINE void delay_ms(u64 ms)	{delay_ticks((tick_t)(ms*param::Hz()/2000));}
		static INLINE void delay_us(u64 us)	{delay_ticks((tick_t)(us*param::MHz()/2));}
		static INLINE void delay_ns(u64 ns)	{delay_ticks((tick_t)(ns*param::MHz()/2000));}

	}

	struct Timer
	{
		enum Unit
		{
			s, ms, us, ns
		};
		
		uint64_t tick_interval;
		Unit unit;

		uint64_t start_tick;
		uint64_t expiration_tick;
		bool is_running;


		Timer(uint64_t _tick_interval, Unit _unit, bool arm_now):
			tick_interval(0),
			unit(Timer::s),
			start_tick(0),
			expiration_tick(0),
			is_running(false)
		{
			this->set_interval(_tick_interval, _unit);
			if (arm_now)
			{
				this->reset();
			}
		}

		void set_interval(uint64_t _tick_interval, Unit _unit)
		{
			this->tick_interval = _tick_interval;
			this->unit = _unit;
			switch (this->unit)
			{
				case s:
					this->tick_interval =  timer::s_to_ticks(_tick_interval)*param::timer_hz();
				case ms:
					this->tick_interval = timer::ms_to_ticks(_tick_interval)*param::timer_hz();
				case us:
					this->tick_interval = timer::us_to_ticks(_tick_interval)*param::timer_hz();
				case ns:
					this->tick_interval = timer::ns_to_ticks(_tick_interval)*param::timer_hz();
			}
		}

		void reset()
		{
			this->start_tick = timer::ticks();
			this->expiration_tick = this->start_tick + this->tick_interval;
			this->is_running = true;
		}

		void kill()
		{
			this->is_running = false;
		}

		bool has_expired()
		{
			if (this->is_running)
			{
				uint64_t time = timer::ticks();
				if (time > this->expiration_tick || (time < this->start_tick && time > this->tick_interval))
				{
					this->kill();
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				// killed
				return true;
			}
		}
	};
}

#endif

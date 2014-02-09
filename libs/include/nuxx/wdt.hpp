#ifndef NUXX_WDT_HPP
#define NUXX_WDT_HPP 1

#include "nu/compiler.h"
#include "nu/wdt.h"

/**
 * Clear WatchDogTimer. Normally set to ~2 seconds.
 */
namespace nu {
	namespace WDT {
		static ALWAYSINLINE void clear()	{nu__WDT__clear();}
		static ALWAYSINLINE void enable_clear()		{nu__WDT__clear_enable();}
		static ALWAYSINLINE void disable_clear()	{nu__WDT__clear_disable();}
		static ALWAYSINLINE void enable()	{nu__WDT__enable();}
		static ALWAYSINLINE void disable()	{nu__WDT__disable();}
	}
}

#endif

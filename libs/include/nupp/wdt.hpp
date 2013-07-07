#ifndef NUPP_WDT_HPP
#define NUPP_WDT_HPP 1

#include "nu/compiler.h"
#include "nu/wdt.h"

/**
 * Clear WatchDogTimer. Normally set to ~2 seconds.
 */
namespace nu {
	namespace WDT {
		static ALWAYSINLINE void clear()	{nu_wdt_clear();}
		static ALWAYSINLINE void enable_clear()		{nu_wdt_clear_enable();}
		static ALWAYSINLINE void disable_clear()	{nu_wdt_clear_disable();}
		static ALWAYSINLINE void enable()	{nu_wdt_enable();}
		static ALWAYSINLINE void disable()	{nu_wdt_disable();}
	}
}

#endif

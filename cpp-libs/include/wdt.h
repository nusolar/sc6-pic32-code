#ifndef NU_WDT_H
#define NU_WDT_H 1

#include "compiler.h"

#include <xc.h>
#include <plib.h>

namespace nu {
	/**
	 * Clear WatchDogTimer. Normally set to ~2 seconds.
	 */
	namespace WDT {
		static ALWAYSINLINE void enabled_clear_wdt (void) { ClearWDT(); }
		static ALWAYSINLINE void disabled_clear_wdt(void) {}
		
		extern void (*clear)(void);
		
		static ALWAYSINLINE void enable_clear(void) {
			clear = enabled_clear_wdt;
		}
		
		static ALWAYSINLINE void disable_clear(void) {
			clear = disabled_clear_wdt;
		}
	}
}

#endif

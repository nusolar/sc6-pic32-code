#ifndef NUPP_WDT_HPP
#define NUPP_WDT_HPP 1

#include "nu/compiler.h"

#include <xc.h>
#include <plib.h>

namespace nu {
	/**
	 * Clear WatchDogTimer. Normally set to ~2 seconds.
	 */
	struct WDT {
		static void (*clear)(void);

		static ALWAYSINLINE void enable_clear() {
			clear = enabled_clear_wdt;
		}

		static ALWAYSINLINE void disable_clear() {
			clear = disabled_clear_wdt;
		}
		
		static ALWAYSINLINE void enable() {
			EnableWDT();
		}
		
		static ALWAYSINLINE void disable() {
			DisableWDT();
		}

	private:
		static void enabled_clear_wdt (void) { ClearWDT(); }
		static void disabled_clear_wdt(void) {}
	};
}

#endif

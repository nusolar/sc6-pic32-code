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

		static ALWAYSINLINE void enable_clear(void) {
			clear = enabled_clear_wdt;
		}

		static ALWAYSINLINE void disable_clear(void) {
			clear = disabled_clear_wdt;
		}

	private:
		static ALWAYSINLINE void enabled_clear_wdt (void) { ClearWDT(); }
		static ALWAYSINLINE void disabled_clear_wdt(void) {}
	};
}

#endif

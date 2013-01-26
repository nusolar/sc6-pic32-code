#include <peripheral/wdt.h>
#include "wdt.h"

static void enabled_clear_wdt (void) { /* ClearWDT(); */}
static void disabled_clear_wdt(void) {}

void (*clear_wdt) (void) = enabled_clear_wdt;

void enable_clear_wdt (void) { clear_wdt = enabled_clear_wdt;  }
void disable_clear_wdt(void) { clear_wdt = disabled_clear_wdt; }

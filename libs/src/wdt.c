#include "wdt.h"
#include <xc.h>
#include <peripheral/wdt.h>

static void enabled_clear_wdt (void) { ClearWDT(); }
static void disabled_clear_wdt(void) {}

void (*nu_wdt_clear) (void) = enabled_clear_wdt;

void nu_wdt_enable_clear(void) { nu_wdt_clear = enabled_clear_wdt;  }
void nu_wdt_disable_clear(void) { nu_wdt_clear = disabled_clear_wdt; }

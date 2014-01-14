#include <xc.h>
#include <peripheral/wdt.h>
#include "nu/wdt.h"

static void enabled_clear_wdt (void) { nu_platform_wdt_clearable(); }
static void disabled_clear_wdt(void) {}

void (*nu__WDT__clear) (void) = enabled_clear_wdt;

void nu__WDT__clear_enable(void) { nu__WDT__clear = enabled_clear_wdt;  }
void nu__WDT__clear_disable(void) { nu__WDT__clear = disabled_clear_wdt; }

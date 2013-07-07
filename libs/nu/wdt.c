#include <xc.h>
#include <peripheral/wdt.h>
#include "nu/wdt.h"

static void enabled_clear_wdt (void) { nu_platform_wdt_clearable(); }
static void disabled_clear_wdt(void) {}

void (*nu_wdt_clear) (void) = enabled_clear_wdt;

void nu_wdt_clear_enable(void) { nu_wdt_clear = enabled_clear_wdt;  }
void nu_wdt_clear_disable(void) { nu_wdt_clear = disabled_clear_wdt; }
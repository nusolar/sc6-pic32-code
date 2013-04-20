#include "wdt.h"

using namespace nu;

void (*WDT::clear)(void) = &WDT::enabled_clear_wdt;
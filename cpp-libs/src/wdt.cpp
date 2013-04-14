#include "wdt.h"

using namespace nu::WDT;

void (*clear)(void) = &enabled_clear_wdt;
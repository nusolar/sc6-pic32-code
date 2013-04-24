#include "nupp/wdt.hpp"

using namespace nu;

void (*WDT::clear)(void) = &WDT::enabled_clear_wdt;

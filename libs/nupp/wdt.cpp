#include "nupp/wdt.hpp"

using namespace nu;

void (*WDT::clear)(void) = WDT::disabled_clear_wdt;

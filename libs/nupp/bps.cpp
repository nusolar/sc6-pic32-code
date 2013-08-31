#include "nupp/bps.hpp"
#include "nupp/wdt.hpp"

nu::BPS nu::bps;

void nu::BPS::main() {
	nu::WDT::disable();
	nu::bps = nu::BPS();
	// start run loop, never return:
	nu::bps.run_loop();
}


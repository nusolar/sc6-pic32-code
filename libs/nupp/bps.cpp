#include "nupp/bps.hpp"
#include "nupp/wdt.hpp"
#include <new>

void nu::BPS::main(nu::BPS *bps) {
	nu::WDT::disable();
	new (bps) nu::BPS();
	// start run loop, never return:
	bps->run_loop();
}


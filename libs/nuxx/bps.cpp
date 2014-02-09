#include "nuxx/board/bps.hpp"
#include "nuxx/wdt.hpp"
#include <new>

void nu::BPS::main(nu::BPS *bps) {
	nu::WDT::disable();
	new (bps) nu::BPS();
	// start run loop, never return:
	bps->run_loop();
}


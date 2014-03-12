#include "nuxx/board/bps.hpp"
#include "nuxx/board/pedals.hpp"
#include "nuxx/board/test.hpp"
#include "nuxx/wdt.hpp"
#include <new>

void nu::Pedals::main(nu::Pedals *self)
{
	nu::WDT::disable();			// enable watchdog timer
	new (self) nu::Pedals();	// initialize variables
	self->setup();				// setup hardware
	self->run_loop();			// start run loop, never return.
}

#if 0
void nu::BPS::main(nu::BPS *self)
{
	nu::WDT::disable();			// enable watchdog timer
	new (self) nu::BPS();		// initialize variables
	self->setup();				// setup hardware
	self->run_loop();			// start run loop, never return.
}
#endif

#if 0
void nu::TestBoard::main(nu::TestBoard *self)
{
	nu::WDT::disable();			// enable watchdog timer
	new (self) nu::TestBoard();	// initialize variables
	self->setup();				// setup hardware
	self->run_loop();			// start run loop, never return.
}
#endif

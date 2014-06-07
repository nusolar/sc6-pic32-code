//#include "nuxx/board/bps.hpp"
#include "nuxx/board/DriverControls.hpp"
//#include "nuxx/board/test.hpp"
#include "nuxx/wdt.hpp"
#include <new>

#if 1
void nu::DriverControls::main(nu::DriverControls *self)
{
	nu::param::Init();			// initialize microcontroller
	nu::WDT::disable();			// enable watchdog timer
	new (self) nu::DriverControls();	// initialize variables
	self->setup();				// setup hardware
	self->run_loop();			// start run loop, never return.
}
#endif

#if 0
void nu::BPS::main(nu::BPS *self)
{
	nu::param::Init();			// initialize microcontroller
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

#include "nuxx/board/bps.hpp"
#include "nuxx/board/pedals.hpp"
#include "nuxx/board/test.hpp"
#include "nuxx/wdt.hpp"
#include <new>
unsigned int debug = 0;

void nu::Pedals::main(nu::Pedals *self)
{
	nu::WDT::disable();
	new (self) nu::Pedals();
	self->setup();
	// start run loop, never return:
	self->run_loop();
}

#if 0
void nu::BPS::main(nu::BPS *self)
{
	nu::WDT::disable();
	new (self) nu::BPS();
	self->setup();
	// start run loop, never return:
	self->run_loop();
}

void nu::TestBoard::main(nu::TestBoard *self)
{
	nu::WDT::disable();
	new (self) nu::TestBoard();
	self->setup();
	// start run loop, never return:
	self->run_loop();
}
#endif

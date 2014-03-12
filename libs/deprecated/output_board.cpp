#include "nuxx/board/output_board.hpp"
#include "nuxx/wdt.hpp"
#include <new>

void nu::OutputBoard::main(nu::OutputBoard *out) {
	nu::WDT::disable();
	new (out) nu::OutputBoard();
	// start run loop, never return:
	out->run_loop();
}

#include "nupp/board/output_board.hpp"
#include "nupp/wdt.hpp"
#include <new>

void nu::OutputBoard::main(nu::OutputBoard *out) {
	nu::WDT::disable();
	new (out) nu::OutputBoard();
	// start run loop, never return:
	out->run_loop();
}

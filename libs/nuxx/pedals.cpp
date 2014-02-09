
#include "nuxx/board/pedals.hpp"
#include <new>

using namespace nu;

void Pedals::main(Pedals *arena) {
	WDT::disable();
	new (arena) Pedals();
	arena->run_loop();
}

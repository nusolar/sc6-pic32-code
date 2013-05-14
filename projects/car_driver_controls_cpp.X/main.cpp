#include "nu/common_pragmas.h"
#include "nupp/driver_controls.hpp"

/** Call DriverControls::main(), NEVER RETURN */
int main(int argc, const char* argv[]) {
	nu::DriverControls::main();
	return 0;
}


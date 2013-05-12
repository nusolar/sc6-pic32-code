#include "nu/platform/common_pragmas.h"
#include "nupp/steering_wheel.hpp"

/** Call SteeringWheel::main(), NEVER RETURN */
int main(int argc, const char* argv[]) {
	nu::SteeringWheel::main();
	return 0;
}

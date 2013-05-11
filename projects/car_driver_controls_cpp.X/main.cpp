#include "nu/platform/common_pragmas.h"
#include "nupp/driver_controls.hpp"

using namespace std;
using namespace nu;

/**
 * Instantiate DriverControls object
 */
int main(int argc, const char* argv[]) {
	DriverControls dc{};
	dc.led1.on();
	while (true) {
		dc.demo();
	}
	return 0;
}


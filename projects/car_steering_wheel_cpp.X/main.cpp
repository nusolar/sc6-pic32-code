#include "nu/platform/common_pragmas.h"
#include "nupp/steering_wheel.hpp"

using namespace std;
using namespace nu;

int main(int argc, const char* argv[]) {
	WDT::enable_clear();
	SteeringWheel sw{};
	sw.animate_leds();
	while (true) {
		sw.run();
	}
	return 0;
}

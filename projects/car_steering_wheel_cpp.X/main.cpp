#include "nu/platform/common_pragmas.h"
#include "nupp/steering_wheel.hpp"

using namespace std;
using namespace nu;
using namespace can;

int main() {
	SteeringWheel sw{};
	sw.animate_leds();
	while (true) {
		sw.demo();
	}
}

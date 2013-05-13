//
//  steering_wheel.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nupp/steering_wheel.hpp"
#include "nupp/allocator.hpp"

using namespace nu;

void SteeringWheel::main() {
	NU_GIFT(4096);
	WDT::enable();
	SteeringWheel sw{};
	sw.animate_leds();
	while (true) {
		sw.run();
	}
}

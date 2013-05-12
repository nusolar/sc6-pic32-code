//
//  steering_wheel.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nupp/steering_wheel.hpp"

using namespace nu;

void SteeringWheel::main() {
	WDT::enable_clear();
	SteeringWheel sw{};
	sw.animate_leds();
	while (true) {
		sw.run();
	}
}

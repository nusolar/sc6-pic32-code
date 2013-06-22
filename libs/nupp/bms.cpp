//
//  bms.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nupp/bms.hpp"
#include "nupp/allocator.hpp"

using namespace nu;

void BatteryMs::main() {
	WDT::enable();
	BatteryMs bms{};
	bms.boot();
	PORTSetBits(IOPORT_D, BIT_2);
	while (true) {
		bms.run();
	}
}

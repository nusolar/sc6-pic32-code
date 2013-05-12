//
//  bms.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nupp/bms.hpp"
#include <alloca.h>

using namespace nu;

void BatteryMs::main() {
	WDT::enable_clear();
	BatteryMs bms{};
	while (true) {
		bms.run();
	}
}

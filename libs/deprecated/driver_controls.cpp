//
//  driver_controls.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nuxx/board/driver_controls.hpp"

using namespace nu;

void DriverControls::main() {
	WDT::disable();
	DriverControls dc{};
	dc.led1.on();
	while (true) {
		dc.test();
	}
}

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

const char *BMS::Trip::name[] = {
	NU_TRIPCODE(NU_ERROR_NAMES)
};

void BMS::main() {
	WDT::disable();
	BMS bms{};
	bms.boot();
	while (true) {
		bms.run();
	}
}

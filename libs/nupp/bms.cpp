//
//  bms.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nupp/board/bms.hpp"

using namespace nu;

const char *BMS::Trip::name[] = {
	NU_TRIPCODE(NU_ERROR_NAMES)
};

void BMS::main() {
	WDT::disable();
	BMS bms = BMS();
	bms.boot();
	while (true) {
		bms.run();
	}
}

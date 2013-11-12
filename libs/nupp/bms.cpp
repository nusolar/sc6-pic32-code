//
//  bms.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nupp/board/bms.hpp"
#include <new>

using namespace nu;

const char *BMS::Trip::name[] = {
	NU_TRIPCODE(NU_ERROR_NAMES)
};

void nu::BMS::main(nu::BMS *bms) {
	nu::WDT::disable();
	new (bms) nu::BMS();
	bms->boot();
	while (true) {
		bms->run();
	}
}

//
//  bms.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 5/11/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "nuxx/board/oldbps.hpp"
#include <new>

using namespace nu;

const char *OldBMS::Trip::name[] = {
	NU_TRIPCODE(NU_ERROR_NAMES)
};

void nu::OldBMS::main(nu::OldBMS *bms) {
	nu::WDT::disable();
	new (bms) nu::OldBMS();
	bms->boot();
	bms->run_loop();
}

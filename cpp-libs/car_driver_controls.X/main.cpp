/*
 * File:   main.cpp
 * Author: alex
 *
 * Created on April 16, 2013, 2:47 AM
 */

#include "nu32.h"
#include "can.h"
#include "pinctl.h"
#include <vector>

namespace nu {
	struct DriverControls: protected Nu32 {
		can::CAN ws_can, common_can;
		std::vector<Pin> analog_ins, digi_ins, digi_outs;

		#define ANALOG_INS\
			_PIN(regen_pedel, B, 0)\
			_PIN(accel_pedel, B, 1)\
			_PIN(airgap_pot, B, 4)

		#define DIGITAL_INS\
			_PIN(brake_pedal,         B, 2)\
			_PIN(headlight_switch,    B, 3)\
			_PIN(airgap_enable,       B, 5)\
			_PIN(regen_enable,        B, 8)\
			_PIN(reverse_switch,      B, 9)

		#define DIGITAL_OUTS\
			_PIN(lights_brake, D, 0)\
			_PIN(lights_l,	   D, 1)\
			_PIN(lights_r,     D, 2)\
			_PIN(headlights,   D, 3)

		enum pin {
		#define _PIN(name, ltr, num) name, //= Pin(IOPORT_##ltr, BIT_##num, #name);
			ANALOG_INS
			DIGITAL_INS
			DIGITAL_OUTS
		#undef _PIN
		};
	};
}


using namespace std;
using namespace nu;

/**
 * Instantiate DriverControls object
 */
int main(int argc, const char* argv[]) {
	return 0;
}

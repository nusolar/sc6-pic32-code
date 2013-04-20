/*
 * File:   main.cpp
 * Author: alex
 *
 * Created on April 16, 2013, 2:47 AM
 */

#include <cstdint>
#include "array.h"

#include "nu32.h"
#include "nokia5110.h"
#include "can.h"
#include "pinctl.h"
#include "wdt.h"

namespace nu {
	struct DriverControls: protected Nu32 {
		Nokia5110 lcd;
		can::CAN ws_can, common_can;
		//std::vector<Pin> analog_ins, digital_ins, digital_outs;
		Enum<Pin, 3> analog_ins, digital_ins, digital_outs;

		/*
		 * Pin definitions
		 */
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
		
		#define _PIN(name, ltr, num) uint16_t name##_k;
			ANALOG_INS
			DIGITAL_INS
			DIGITAL_OUTS
		#undef _PIN
		
		DriverControls(): Nu32(Nu32::V2, HZ), ws_can(CAN1), common_can(CAN2),
			lcd(IOPORT_G, BIT_9, SPI_CHANNEL2, IOPORT_A, BIT_9, IOPORT_E, BIT_9)
		{
			#define _PIN(name, ltr, num) name##_k = _ITER.enumerate(Pin(IOPORT_##ltr, BIT_##num, #name));
				#define _ITER analog_ins
					ANALOG_INS
				#undef _ITER
				#define _ITER digital_ins
					DIGITAL_INS
				#undef _ITER
				#define _ITER digital_outs
					DIGITAL_OUTS
				#undef _ITER
			#undef _PIN
		}
		
		void setup() {
			WDT::clear();
			common_can.setup_easy((CAN_MODULE_EVENT)0, INT_PRIORITY_DISABLED);
			common_can.add_rx(CAN_CHANNEL0, 32, CAN_RX_FULL_RECEIVE);
			common_can.add_tx(CAN_CHANNEL1, 32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY);
			common_can.add_tx(CAN_CHANNEL2, 32, CAN_TX_RTR_DISABLED, CAN_LOWEST_PRIORITY); // err chn
			
			ws_can.setup_easy((CAN_MODULE_EVENT)0, INT_PRIORITY_DISABLED);
			ws_can.add_tx(CAN_CHANNEL1, 32, CAN_TX_RTR_DISABLED, CAN_HIGH_MEDIUM_PRIORITY);
			
			lcd.setup();
		}
	};
}


using namespace std;
using namespace nu;

/**
 * Instantiate DriverControls object
 */
int main(int argc, const char* argv[]) {
	WDT::clear();
	DriverControls dc{};
	dc.setup();
	return 0;
}


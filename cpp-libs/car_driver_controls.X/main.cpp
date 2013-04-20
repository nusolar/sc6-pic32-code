/*
 * File:   main.cpp
 * Author: alex
 *
 * Created on April 16, 2013, 2:47 AM
 */

#include <cstdint>
#include "array.h"
#include "timer.h"

#include "nu32.h"
#include "nokia5110.h"
#include "can.h"
#include "pinctl.h"
#include "wdt.h"

namespace nu {
	namespace value {
		enum value
		{lights_head = 0, lights_brake, lights_l, lights_r,
			accel, regen, reverse, LEN};
	}
	struct DriverControls: protected Nu32 {
		Nokia5110 lcd;
		can::CAN ws_can, common_can;
		Enum<Pin, 3> analog_ins, digital_ins, digital_outs;
		
		struct values {
			bool lights_head, lights_brake, lights_l, lights_r,
				accel_en, brake_en, reverse_en, regen_en, airgap_en;
			float accel, regen, airgap;
		} values;
		
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
			WDT::clear();
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
			
			// TODO: configure ADC10
			lcd.setup();
		}
		
		void read_ins() {
			WDT::clear();
			values.accel = ((float)ReadADC10(1) + 0)/1024; // scale 0-1023 to 0-1
			if (values.accel < 0) values.accel = 0; // TODO: print warning, clamp
			if (values.accel > 1) values.accel = 1; // TODO: print warning
			values.accel_en = values.accel > 0.05;
			
			values.lights_head	= digital_ins[headlight_switch_k].read()? 1: 0;
			values.brake_en		= digital_ins[brake_pedal_k].read()? 1: 0;
			values.lights_brake = values.brake_en;
			values.reverse_en	= digital_ins[reverse_switch_k].read();
			values.regen_en		= digital_ins[regen_enable_k].read();
			values.regen		= digital_ins[regen_pedel_k].read(); // TODO: regen_pot?
			values.airgap_en	= digital_ins[airgap_enable_k].read();
			values.airgap		= digital_ins[airgap_pot_k].read();
		}
		
		void recv_can() {}
		
		void set_lights() {
			WDT::clear();
			digital_outs[headlights_k]		&= values.lights_head;
			digital_outs[lights_brake_k]	&= values.lights_brake;
			digital_outs[lights_l_k]		&= values.lights_l;
			digital_outs[lights_l_k]		&= values.lights_l;
		}
		
		void set_motor() {
			WDT::clear();
			can::frame::ws20::rx::drive_cmd drive {0, 0}; // [current, velocity]
			
			if (values.brake_en) {
				if (values.regen_en) 
					drive = {0.2, 0}; // REGEN_AMOUNT
				else 
					drive = {0, 0};
			} else if (values.accel_en)
				drive = {values.accel, 100};
			
			if (values.reverse_en)
				drive.motorVelocity *= -1;
			
			led1.on(); delay_ms(100); led1.off(); // WTF
			ws_can.tx(&drive, sizeof(drive), 0); // ERROR: CAN ADDRESS?
		}
		
		void run() {
			WDT::clear();
			
			read_ins();
			set_lights();
			set_motor();
			
			lcd.lcd_clear();
			lcd.goto_xy(0, 0);
			lcd.printf("%f", 25);
		}
	};
}


using namespace std;
using namespace nu;

/**
 * Instantiate DriverControls object
 */
int main(int argc, const char* argv[]) {
	DriverControls dc{};
	dc.setup();
	while (true) {
		dc.run();
	}
	return 0;
}


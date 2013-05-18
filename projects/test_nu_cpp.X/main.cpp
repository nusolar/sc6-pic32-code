/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on May 16, 2013, 11:58 PM
 */

#include "nu/platform/common_pragmas.h"
#include "nupp/nu32.hpp"

extern "C" {
	// declared static in case exception condition would prevent
	// auto variable being created
	static enum exceptions {
		EXCEP_IRQ = 0,          // interrupt
		EXCEP_AdEL = 4,         // address error exception (load or ifetch)
		EXCEP_AdES,             // address error exception (store)
		EXCEP_IBE,              // bus error (ifetch)
		EXCEP_DBE,              // bus error (load/store)
		EXCEP_Sys,              // syscall
		EXCEP_Bp,               // breakpoint
		EXCEP_RI,               // reserved instruction
		EXCEP_CpU,              // coprocessor unusable
		EXCEP_Overflow,         // arithmetic overflow
		EXCEP_Trap,             // trap (possible divide by zero)
		EXCEP_IS1 = 16,         // implementation specfic 1
		EXCEP_CEU,              // CorExtend Unuseable
		EXCEP_C2E               // coprocessor 2
	} _excep_code;



	static unsigned int _epc_code;
	static unsigned int _excep_addr;

	// this function overrides the normal _weak_ generic handler
	void _general_exception_handler(void)
	{
		asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
		asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

		_excep_code = (exceptions) ((_excep_code & 0x0000007C) >> 2);

		while (1) {
			// Examine _excep_code to identify the type of exception
			// Examine _excep_addr to find the address that caused the exception
		}
	}
}

using namespace nu;

int main() {
	Nu32 nu32(Nu32::V1);
	nu32.led1.on();
	nu32.led2.on();
	while (true) {
		nu32.led1.toggle();
		timer::delay_s<1>();
	}
	return 0;
}

/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on July 5, 2013, 7:34 PM
 *
 * Mainfile for Output Board.
 */

#include "nu/common_pragmas.h"
#include "nupp/output_board.hpp"
#include <cstdint>

#define div_roundup(DIVIDEND, DIVISOR) ((long long)((DIVIDEND)/(DIVISOR)) + (((DIVIDEND)%(DIVISOR)>0)? 1: 0))

// Allocate twice the space of an nu::OutputBoard, rounding up.
uint64_t arena[div_roundup(sizeof(nu::OutputBoard), 4)] ALIGNED(__BIGGEST_ALIGNMENT__);

void kill() {
	((nu::OutputBoard *)arena)->emergency_shutoff();
}

extern "C" {
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

	static unsigned int _excep_addr;

	// this function overrides the normal _weak_ generic handler
	void _general_exception_handler(void)
	{
		asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
		asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

		_excep_code = (exceptions) ((_excep_code & 0x0000007C) >> 2);

		kill();

		while (1) {
			Nop();
			// Examine _excep_code to identify the type of exception
			// Examine _excep_addr to find the address that caused the exception
		}
	}
}

const char *uuid = __DATE__ " " __TIME__;

/** Call OutputBoard::main(), NEVER RETURN. */
int main() {
	nu::OutputBoard::main((nu::OutputBoard *)arena);
	return 0;
}


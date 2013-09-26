#include "nu/common_pragmas.h"
#include "nupp/board/bps.hpp"
#include <cstdint>

#define div_roundup(DIVIDEND, DIVISOR) ((long long)((DIVIDEND)/(DIVISOR)) + (((DIVIDEND)%(DIVISOR)>0)? 1: 0))

// Allocate twice the space of an nu::BPS, rounding up.
uint64_t arena[div_roundup(sizeof(nu::BPS), 4)] ALIGNED(__BIGGEST_ALIGNMENT__);

extern "C" {
	void kill(void); // implemented below, in C++

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

void kill() {
	((nu::BPS *)arena)->emergency_shutoff();
}

/** Call BPS::main(), NEVER RETURN */
int main(){
	nu::BPS::main((nu::BPS *)arena);
    return 0;
}

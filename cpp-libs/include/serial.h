//
//  serial.h
//  nusolar_lib
//
//  Created by Al Chandel on 4/13/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#ifndef __nusolar_lib__serial__
#define __nusolar_lib__serial__

#include "compiler.h"
#include <cstdint>

#include <cstring> // strlen
#include <peripheral/uart.h>

namespace nu {
	struct Serial {
		enum module_interrupt {
			NU_USE_UART_INTERRUPT,
			NU_NOT_USE_UART_INTERRUPT
		};
		
		UART_MODULE module;
		
		INLINE Serial(UART_MODULE _mod): module(_mod) {}
		
		void setup(uint32_t baud, module_interrupt use_interrupt,
				   INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
				   UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
				   UART_ENABLE_MODE enable_modes);
		
		void rx(void *dst, size_t n);
		void tx(const void *src, size_t n);
		void printf(const char *fmt, ...); // PRINTF(2, 3) 
		void ALWAYSINLINE puts(const char *str) {
			tx(str, strlen(str));
		}
	};
}

#endif /* defined(__nusolar_lib__serial__) */

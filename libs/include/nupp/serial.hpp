#ifndef NUPP_SERIAL_HPP
#define NUPP_SERIAL_HPP 1

#include "nupp/compiler.hpp"
#include <cstdint>

#include <cstring> // strlen
#include <plib.h>

namespace nu {
	/**
	 * Encapsulate Serial/UART reading/writing.
	 */
	struct Serial {
		enum module_interrupt {
			USE_UART_INTERRUPT,
			NOT_USE_UART_INTERRUPT
		};
		
		UART_MODULE module;
		
		ALWAYSINLINE Serial(UART_MODULE _mod): module(_mod) {}
		virtual ~Serial() {}
		
		void setup(uint32_t baud, module_interrupt use_interrupt,
				   INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
				   UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
				   UART_ENABLE_MODE enable_modes);
		
		void rx(void *dst, size_t n);
		void tx(const void *src, size_t n);
		void PRINTF(2, 3) printf(const char *fmt, ...);
		void ALWAYSINLINE puts(const char *str) {
			tx(str, strlen(str));
		}
	};
}

#endif

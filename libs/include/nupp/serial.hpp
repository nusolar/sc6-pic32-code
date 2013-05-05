#ifndef NUPP_SERIAL_HPP
#define NUPP_SERIAL_HPP 1

#include "nu/compiler.h"
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

		Serial(UART_MODULE _mod, uint32_t baud,
				   module_interrupt use_interrupt = NOT_USE_UART_INTERRUPT,
				   INT_PRIORITY int_priority = INT_PRIORITY_DISABLED,
				   UART_FIFO_MODE interrupt_modes = (UART_FIFO_MODE)0,
				   UART_LINE_CONTROL_MODE line_control_modes = (UART_LINE_CONTROL_MODE)0,
				   UART_CONFIGURATION uart_config = (UART_CONFIGURATION)0,
				   UART_ENABLE_MODE enable_modes = (UART_ENABLE_MODE)(UART_ENABLE|UART_RX|UART_TX));
		virtual ~Serial() {}
		
		
		void rx(void *dst, size_t n);
		void tx(const void *src, size_t n);
		ALWAYSINLINE virtual void puts(const char *str) {
			tx(str, strlen(str));
		}
		
		DEPRECATED void PRINTF(2, 3) printf(const char *fmt, ...);
	};
}

#endif

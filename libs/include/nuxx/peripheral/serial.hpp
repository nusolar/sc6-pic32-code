#ifndef NUXX_SERIAL_HPP
#define NUXX_SERIAL_HPP 1

#include "nuxx/param.hpp"
#include "nuxx/wdt.hpp"
#include "nuxx/stream.hpp"
#include "nu/compiler.h"
#include "nu/platform.h"
#include <cstdint>
#include <cstring> // strlen




#if NU_PLATFORM==NU_PLATFORM_PIC32MX /* PIC32MX-specific Serial code */
extern "C" {
#include <plib.h>
}

// To specify a UART, use UART(NUMBER)
#define UART(N) PlatformUart(UART##N, 115200)

namespace nu {
	/**
	 * Initialize with UART(N) macro.
	 */
	struct PlatformUart {
		UART_MODULE module;
		uint32_t baud;

		PlatformUart(UART_MODULE _mod, uint32_t _baud):
			module(_mod), baud(_baud) {}

		void setup(bool use_interrupt = false,
				   INT_PRIORITY int_priority = INT_PRIORITY_DISABLED,
				   UART_CONFIGURATION uart_config = (UART_CONFIGURATION)UART_ENABLE_PINS_TX_RX_ONLY,
				   UART_FIFO_MODE interrupt_modes = (UART_FIFO_MODE)0,
				   UART_LINE_CONTROL_MODE line_control_modes = (UART_LINE_CONTROL_MODE)(UART_DATA_SIZE_8_BITS|UART_PARITY_NONE|UART_STOP_BITS_1),
				   UART_ENABLE_MODE enable_modes = (UART_ENABLE_MODE)(UART_PERIPHERAL|UART_RX|UART_TX))
		{
			UARTConfigure(module, uart_config);
			UARTSetFifoMode(module, interrupt_modes);
			UARTSetLineControl(module, line_control_modes);
			UARTSetDataRate(module, param::pbus_hz(), baud);
			UARTEnable(module, (UART_ENABLE_MODE) UART_ENABLE_FLAGS(enable_modes));

			if (use_interrupt) {
				INT_VECTOR int_vect;
				INT_SOURCE int_src;
				switch(module) {
					case UART1:
						int_vect = INT_UART_1_VECTOR;
						int_src = INT_U1RX;
						break;
					case UART2:
						int_vect = INT_UART_2_VECTOR;
						int_src = INT_U2RX;
						break;
					case UART3:
						int_vect = INT_UART_3_VECTOR;
						int_src = INT_U3RX;
						break;
					case UART4:
						int_vect = INT_UART_4_VECTOR;
						int_src = INT_U4RX;
						break;
					case UART5:
						int_vect = INT_UART_5_VECTOR;
						int_src = INT_U5RX;
						break;
					case UART6:
						int_vect = INT_UART_6_VECTOR;
						int_src = INT_U6RX;
						break;
					case UART_NUMBER_OF_MODULES:
					default:
						return; // WTF
				}
				INTEnable(int_src, INT_ENABLED);
				INTSetVectorPriority(int_vect, int_priority);
				INTSetVectorSubPriority(int_vect, INT_SUB_PRIORITY_LEVEL_0);
			}
		}

		virtual ~PlatformUart() {}

		void tx(const void *src, size_t n) {
			size_t ui;
			for (ui = 0; ui < n; ++ui) {
				WDT::clear();
				while (!UARTTransmitterIsReady(module)) Nop();
				UARTSendDataByte(module, ((const BYTE *)src)[ui]);
			}
			//	while (!UARTTransmissionHasCompleted(module)) Nop(); // TODO: async
		}

		int rx(void *dst, size_t n) {
			size_t ui;
			for (ui = 0; ui < n; ++ui) {
				if (!UARTReceivedDataIsAvailable(module)) {
					return -1;
				}
				WDT::clear();
				((uint8_t *)dst)[ui] = UARTGetDataByte(module);
			}
			return 0;
		}
	};
}
#endif /* End PLATFORM specific code */




namespace nu {
	/**
	 * Encapsulate Serial/UART reading/writing.
	 */
	struct Serial: public OStream {
		PlatformUart base;

		Serial(PlatformUart _module): OStream(), base(_module) {}

		void setup()	{this->base.setup();}

		int rx(void *dst, size_t n)		{return this->base.rx(dst, n);}
		void tx(const void *src, size_t n)	{this->base.tx(src, n);}

		/** Required to implement OStream: */
		void puts(const char *str) {
			this->tx(str, strlen(str));
		}

		/** Returns # of bytes copied. */
		size_t read_line(void *dst, size_t n) {
			for (size_t ui=0; ui < n; ++ui) {
				uint8_t buffer = 0;
				int error = this->base.rx(&buffer, 1);

				if (error < 0) {
					// Data unavailable, return # of bytes copied thusfar, EXCLUDING this final attempt.
					return ui;
				} else {
					// write byte to destination;
					((uint8_t *)dst)[ui] = buffer;
				}

				if (buffer == '\n') {
					// ENDL, return # of bytes copied, INCLUDING this attempt
					return ui+1;
				}
			}
			return n;
		}
	};
}

#endif

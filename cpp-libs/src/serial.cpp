//
//  serial.cpp
//  nusolar_lib
//
//  Created by Al Chandel on 4/13/13.
//  Copyright (c) 2013 Alex Chandel. All rights reserved.
//

#include "serial.h"
#include "param.h"
#include "wdt.h"
#include <alloca.h>
#include <stdarg.h>
#include <stdio.h>

using namespace nu;

void Serial::setup(__uint32_t baud, nu::Serial::module_interrupt use_interrupt,
				   INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
				   UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
				   UART_ENABLE_MODE enable_modes) {
	INT_VECTOR int_vect;
    INT_SOURCE int_src;
	
    UARTConfigure(module, uart_config);
    UARTSetFifoMode(module, interrupt_modes);
    UARTSetLineControl(module, line_control_modes);
    UARTSetDataRate(module, HZ, baud);
    UARTEnable(module, (UART_ENABLE_MODE) UART_ENABLE_FLAGS(enable_modes));
	
    if (NU_USE_UART_INTERRUPT == use_interrupt) {
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
				return; // WTF
            default:
                break;
        }
        INTEnable(int_src, INT_ENABLED);
        INTSetVectorPriority(int_vect, int_priority);
        INTSetVectorSubPriority(int_vect, INT_SUB_PRIORITY_LEVEL_0);
    }
}

void Serial::tx(const void *src, size_t n) {
	size_t ui;
	for (ui = 0; ui < n; ++ui) {
		WDT::clear();
		while (!UARTTransmitterIsReady(module))
			Nop();
		UARTSendDataByte(module, ((const BYTE *)src)[ui]);
    }
}

void Serial::rx(void *dst, size_t n) {
	size_t ui;
	for (ui = 0; ui < n && UARTReceivedDataIsAvailable(module); ++ui) {
		((uint8_t *)dst)[ui] = UARTGetDataByte(module);
	}
}

/*
void Serial::printf(const char *fmt, ...) {
	char *buf;
	size_t n;
	va_list fmtargs;
	va_start(fmtargs, fmt);
	n = vsnprintf(NULL, (size_t)0, fmt, fmtargs);
	buf = (char *) alloca(n);
	vsprintf(buf, fmt, fmtargs);
	va_end(fmtargs);
	tx(buf, n);
}*/


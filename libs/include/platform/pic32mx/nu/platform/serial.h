#ifndef NU_SERIAL_PIC32MX_H
#define NU_SERIAL_PIC32MX_H 1

#include <peripheral/uart.h>

struct nu_serial_platform {
    UART_MODULE module;
};

#define NU_SERIAL_UART1 UART1
#define NU_SERIAL_UART2 UART2

#endif


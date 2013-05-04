#ifndef NU_SERIAL_PIC32MX_H
#define NU_SERIAL_PIC32MX_H 1

#include "nu/serial.h"
#include "nu/param.h"
#include "nu/wdt.h"
#include <peripheral/uart.h>


enum nu_serial_module_interrupt {
    NU_USE_UART_INTERRUPT,
    NU_NOT_USE_UART_INTERRUPT
};

struct nu_serial_platform_setup_args {
    enum nu_serial_module_interrupt use_interrupt;
    INT_PRIORITY int_priority;
    UART_FIFO_MODE interrupt_modes;
    UART_LINE_CONTROL_MODE line_control_modes;
    UART_CONFIGURATION uart_config;
    UART_ENABLE_MODE enable_modes;
    UART_MODULE module;
};

struct nu_serial_platform {
    UART_MODULE module;
};

static void
nu_serial_platform_setup(struct nu_serial_platform *platform, u32 baud,
	struct nu_serial_platform_setup_args *arg)
{
    UARTConfigure(platform->module, arg->uart_config);
    UARTSetFifoMode(platform->module, arg->interrupt_modes);
    UARTSetLineControl(platform->module, arg->line_control_modes);
    UARTSetDataRate(platform->module, HZ, baud);
    UARTEnable(platform->module, (UART_ENABLE_MODE) UART_ENABLE_FLAGS(arg->enable_modes));

    if (NU_USE_UART_INTERRUPT == arg->use_interrupt) {
        INT_VECTOR int_vect;
        INT_SOURCE int_src;
        switch(platform->module) {
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
                return;
        }
        INTEnable(int_src, INT_ENABLED);
        INTSetVectorPriority(int_vect, arg->int_priority);
        INTSetVectorSubPriority(int_vect, INT_SUB_PRIORITY_LEVEL_0);
    }
}

static size_t
nu_serial_platform_tx(const struct nu_serial_platform *platform, const void *src, size_t n)
{
    size_t ui;
    for (ui = 0; ui < n; ++ui) {
        nu_wdt_clear();
        while (!UARTTransmitterIsReady(platform->module))
            Nop();
        UARTSendDataByte(platform->module, ((const BYTE *)src)[ui]);
    }
    return n;
}

static size_t
nu_serial_platform_rx(const struct nu_serial_platform *platform, void *dst, size_t n)
{
    size_t ui;
    for (ui = 0; ui < n && UARTReceivedDataIsAvailable(platform->module); ++ui)
        ((u8 *)dst)[ui] = UARTGetDataByte(platform->module);
    return n;
}

#define NU_SERIAL_UART1 UART1
#define NU_SERIAL_UART2 UART2

#endif


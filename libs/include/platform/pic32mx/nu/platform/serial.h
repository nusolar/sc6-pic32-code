#ifndef NU_SERIAL_PIC32MX_H
#define NU_SERIAL_PIC32MX_H 1

#include "nu/compiler.h"
#include <peripheral/uart.h>

extern const struct nu_serial_platform_ops nu_serial_platform_ops;

typedef struct nu_serial_platform {
    UART_MODULE module;
} nu_init_serial_platform_args_t;

#define NU_SERIAL_PLATFORM_INIT(mod) {mod}

#define NU_SERIAL_UART1 {UART1}
#define NU_SERIAL_UART2 {UART2}
#define NU_SERIAL_UART3 {UART2}
#define NU_SERIAL_UART4 {UART2}

static ALWAYSINLINE void
NU_INIT_SERIAL_PLATFORM(struct nu_serial_platform *p,
        const nu_init_serial_platform_args_t *args)
{
    p->module = args->module;
}

enum nu_serial_module_interrupt {
    NU_USE_UART_INTERRUPT,
    NU_NOT_USE_UART_INTERRUPT
};

static const struct nu_serial_platform_setup_args {
    enum nu_serial_module_interrupt use_interrupt;
    INT_PRIORITY int_priority;
    UART_FIFO_MODE interrupt_modes;
    UART_LINE_CONTROL_MODE line_control_modes;
    UART_CONFIGURATION uart_config;
    UART_ENABLE_MODE enable_modes;
} nu_serial_platform_setup_defaults = {
    NU_NOT_USE_UART_INTERRUPT,
    INT_PRIORITY_DISABLED,
    (UART_FIFO_MODE)0,
    (UART_LINE_CONTROL_MODE)0,
    (UART_CONFIGURATION)0,
    (UART_ENABLE_MODE)(UART_ENABLE|UART_TX|UART_RX),
};

#endif


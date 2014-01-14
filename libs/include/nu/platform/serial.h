#ifndef NU_PLATFORM_SERIAL_H
#define NU_PLATFORM_SERIAL_H 1

#include "nu/compiler.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/serial.h"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC
#error "No generic serial code!"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX
#include <peripheral/uart.h>

extern const struct nu__Serial__PlatformOps nu__Serial__platform_ops;

typedef struct nu__Serial__Platform {
    UART_MODULE module;
} nu_init_serial_platform_args_t;

#define NU_SERIAL_PLATFORM_INIT(mod) {mod}

#define NU_SERIAL_UART1 {UART1}
#define NU_SERIAL_UART2 {UART2}
#define NU_SERIAL_UART3 {UART2}
#define NU_SERIAL_UART4 {UART2}

static ALWAYSINLINE void
NU_INIT_SERIAL_PLATFORM(struct nu__Serial__Platform *p,
        const nu_init_serial_platform_args_t *args)
{
    p->module = args->module;
}

enum nu__Serial__ModuleInterrupt {
    NU_USE_UART_INTERRUPT,
    NU_NOT_USE_UART_INTERRUPT
};

static const struct nu__Serial__PlatformSetupArgs {
    enum nu__Serial__ModuleInterrupt use_interrupt;
    INT_PRIORITY int_priority;
    UART_FIFO_MODE interrupt_modes;
    UART_LINE_CONTROL_MODE line_control_modes;
    UART_CONFIGURATION uart_config;
    UART_ENABLE_MODE enable_modes;
} nu__Serial__platform_setup_defaults = {
    NU_NOT_USE_UART_INTERRUPT,
    INT_PRIORITY_DISABLED,
    (UART_FIFO_MODE)0,
    (UART_LINE_CONTROL_MODE)0,
    (UART_CONFIGURATION)0,
    (UART_ENABLE_MODE)(UART_ENABLE|UART_TX|UART_RX),
};

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_SERIAL_H */


#ifndef NU_SERIAL_H
#define NU_SERIAL_H

#include "compiler.h"
#include "error_reporting.h"
#include "nu_types.h"
#include <peripheral/uart.h>

enum module_interrupt {
    UART_INT_ENABLE,
    UART_INT_DISABLE
};

struct serial {
    struct nu_error_reporting_dev erd;
    UART_MODULE module;
};

#define erd_to_serial(erdp) \
    container_of((erdp), struct serial, erd)

WEAK extern const struct nu_vtbl_error_reporting_dev serial_erd_ops;

#define SERIAL_ERD_INIT(min_priority, mod) \
    { \
    NU_ERD_INIT(min_priority, &serial_erd_ops), \
    (mod) \
    }
#define SERIAL_ERD(name, min_priority, mod) \
    struct serial name = SERIAL_ERD_INIT(min_priority, mod)
#define SERIAL_INIT(mod)    SERIAL_ERD_INIT(NU_REP_DEBUG, mod)
#define SERIAL(name, mod) \
    struct serial name = SERIAL_INIT(mod)

void
serial_setup(struct serial *s, u32 baud, enum module_interrupt use_interrupt,
        INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
        UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
        UART_ENABLE_MODE enable_modes);

void PRINTF(2, 3)
serial_printf(const struct serial *s, const char *fmt, ...);

void
serial_tx(const struct serial *s, const void *src, size_t n);

static ALWAYSINLINE void
serial_puts(const struct serial *s, const char *str)
{
    serial_tx(s, str, strlen(str));
}

void
serial_rx(struct serial *s, void *dst, size_t n);

#endif

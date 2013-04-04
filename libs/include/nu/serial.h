#ifndef NU_SERIAL_H
#define NU_SERIAL_H 1

#include "compiler.h"
#include "error_reporting.h"
#include "nu_types.h"
#include <peripheral/uart.h>

enum nu_serial_module_interrupt {
    UART_INT_ENABLE,
    UART_INT_DISABLE
};

struct nu_serial {
    struct nu_error_reporting_dev erd;
    UART_MODULE module;
};

#define nu_erd_to_serial(erdp) \
    container_of((erdp), struct serial, erd)

WEAK extern const struct nu_vtbl_error_reporting_dev nu_serial_erd_ops;

#define NU_SERIAL_ERD_INIT(min_priority, mod) \
    { \
    NU_ERD_INIT(min_priority, &serial_erd_ops), \
    (mod) \
    }
#define NU_SERIAL_ERD(name, min_priority, mod) \
    struct nu_serial name = NU_SERIAL_ERD_INIT(min_priority, mod)
#define NU_SERIAL_INIT(mod)    NU_SERIAL_ERD_INIT(NU_REP_DEBUG, mod)
#define NU_SERIAL(name, mod) \
    struct nu_serial name = NU_SERIAL_INIT(mod)

static ALWAYSINLINE void
NU_INIT_SERIAL(struct nu_serial *s, UART_MODULE mod)
{
    s->module = mod;
}

void
nu_serial_setup(struct nu_serial *s, u32 baud, enum nu_serial_module_interrupt use_interrupt,
        INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
        UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
        UART_ENABLE_MODE enable_modes);

void PRINTF(2, 3)
nu_serial_printf(const struct nu_serial *s, const char *fmt, ...);

void
nu_serial_tx(const struct nu_serial *s, const void *src, size_t n);

static ALWAYSINLINE void
nu_serial_puts(const struct nu_serial *s, const char *str)
{
    (void)s;
    (void)str;
    /* serial_tx(s, str, strlen(str)); */
}

void
nu_serial_rx(struct nu_serial *s, void *dst, size_t n);

#endif

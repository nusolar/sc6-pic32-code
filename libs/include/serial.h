#ifndef NU_SERIAL_H
#define NU_SERIAL_H

#include <peripheral/uart.h>
#include "compiler.h"
#include "error_reporting.h"
#include "nu_types.h"
/* #include "serial_async.h" */

#ifndef SERIAL_MAX_DELIMS
# define SERIAL_MAX_DELIMS  5
#endif

enum module_interrupt {
    NO_UART_INTERRUPT,
    USE_UART_INTERRUPT,
};

struct serial {
    struct error_reporting_dev erd;
    char        delims[SERIAL_MAX_DELIMS];
    size_t      num_delims;
    UART_MODULE module;
};

#define erd_to_serial(erdp) \
    container_of((erdp), struct serial, erd)

extern const struct vtbl_error_reporting_dev serial_erd_ops;

#define SERIAL_ERD_INIT(min_priority, mod)              \
    {                                                   \
    .erd = ERD_INIT(min_priority, &serial_erd_ops),     \
    .delims = {'\r', '\n'},                             \
    .num_delims = 2,                                    \
    .module = mod                                       \
    }
#define SERIAL_ERD(name, min_priority, mod)  \
    struct serial name = SERIAL_ERD_INIT(min_priority, mod)
#define SERIAL_INIT(mod)    \
    {                       \
    .delims = {'\r', '\n'}, \
    .num_delims = 2,        \
    .module = mod           \
    }
#define SERIAL(name, mod)   \
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

s32
serial_rx(struct serial *s, void *dst, size_t n);

s32
serial_flush_rx_buf(struct serial *s, void *dst, size_t n);

#endif

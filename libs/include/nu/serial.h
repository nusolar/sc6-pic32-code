#ifndef NU_SERIAL_H
#define NU_SERIAL_H 1

#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include "nu/utility/data.h"
#include "nu/platform/serial.h"
/*
 * NU_SERIAL_PLATFORM_INIT(...), which expands to {..., ...}
 * NU_INIT_SERIAL_PLATFORM(struct nu_serial_platform *p, ...)
 * NU_SERIAL_PLATFORM(name, ...)
 */

#define nu_erd_to_serial(erdp) \
    container_of((erdp), struct nu_serial, erd)

struct nu_serial {
    struct nu_serial_platform platform;
    struct nu_error_reporting_dev erd;
};

void
nu_serial_setup(struct nu_serial *s, u32 baud,
	struct nu_serial_platform_setup_args *arg);

void
nu_serial_puts(const struct nu_serial *s, const char *str);

void PRINTF(2, 3)
nu_serial_printf(const struct nu_serial *s, const char *fmt, ...);

size_t
nu_serial_rx(const struct nu_serial *s, void *dst, size_t n);

size_t
nu_serial_tx(const struct nu_serial *s, const void *src, size_t n);

#if 0
#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include "nu/nu_types.h"
#include <peripheral/uart.h>

struct nu_serial {
    struct nu_error_reporting_dev erd;
    UART_MODULE module;
};

WEAK extern const struct nu_vtbl_error_reporting_dev nu_serial_erd_ops;

#define NU_SERIAL_ERD_INIT(min_priority, mod) \
    { \
    NU_ERD_INIT(min_priority, &nu_serial_erd_ops), \
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
nu_serial_rx(const struct nu_serial *s, void *dst, size_t n);
#endif

#endif

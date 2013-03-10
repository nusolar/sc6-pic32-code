#ifndef NU_SERIAL_ASYNC_H
#define NU_SERIAL_ASYNC_H

#include "async_io.h"
#include "compiler.h"
#include "circ_buf.h"
#include "error_reporting.h"
#include "nu_types.h"
#include "serial.h"

struct serial_async {
    struct serial serial;
    struct io_async async;
};

#define serial_to_async(s)  container_of(s, struct serial_async, serial)

extern const char *default_delims;
extern const struct vtbl_error_reporting_dev serial_async_erd_ops;

#define SERIAL_ASYNC_ERD_INIT(min_priority, mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz)    \
    {                                                               \
    .serial = SERIAL_ERD_INIT(min_priority, mod),                   \
    .serial.erd = ERD_INIT(min_priority, serial_async_erd_ops),     \
    .rx_delims = default_delims,                                    \
    .tx_buf = CIRC_BUF_INIT((_tx_buf), 0, 0),                       \
    .tx_buf_size = (_tx_buf_siz),                                   \
    .rx_buf = CIRC_BUF_INIT((_rx_buf), 0, 0)                        \
    .rx_buf_size = (_rx_buf_siz)                                    \
    }
#define SERIAL_ASYNC_INIT(mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz)  \
    SERIAL_ASYNC_ERD_INIT(REP_DEBUG, mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz)

#define SERIAL_ASYNC_ERD(name, min_priority, mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz) \
    struct serial_async name = SERIAL_ASYNC_ERD_INIT(min_priority, mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz)
#define SERIAL_ASYNC(name, mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz) \
    SERIAL_ASYNC_ERD(name, REP_DEBUG, mod, _tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz)

static ALWAYSINLINE void
serial_async_setup(struct serial_async *s, u32 baud, enum module_interrupt use_interrupt,
        INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
        UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
        UART_ENABLE_MODE enable_modes)
{
    serial_setup(&(s->serial), baud, use_interrupt, int_priority,
            interrupt_modes, line_control_modes, uart_config, enable_modes);
}

/* returns # of characters written */
s32
serial_async_tx(struct serial_async *s, const void *src, size_t n, bool overrun, bool *overrun_out);

static ALWAYSINLINE void
serial_async_puts(struct serial_async *s, const char *str, bool overrun, bool *overrun_out)
{
    serial_async_tx(s, str, strlen(str), overrun, overrun_out);
}

void PRINTF(4,5)
serial_async_printf(struct serial_async *s, bool overrun, bool *overrun_out, const char *fmt, ...);

void
serial_async_rx(struct serial_async *s, void *dst, size_t n);

/* this command won't return until all data in the async
 * tx buffer is sent
 */
void
serial_async_tx_flush(struct serial_async *s);

#endif

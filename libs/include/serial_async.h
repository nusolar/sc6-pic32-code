#ifndef NU_SERIAL_ASYNC_H
#define NU_SERIAL_ASYNC_H

#include "serial.h"

#ifndef SERIAL_RX_BUF_SIZE
# define SERIAL_RX_BUF_SIZE 500
#endif
#ifndef SERIAL_TX_BUF_SIZE
# define SERIAL_TX_BUF_SIZE 2000
#endif

struct serial_async {
    struct serial serial;
    struct {
        struct {
            struct {
                char *tx;
                char *rx;
            } read;
            struct {
                char *tx;
                char *rx;
            } write;
        } pos;
        char tx[SERIAL_TX_BUF_SIZE];
        char rx[SERIAL_RX_BUF_SIZE];
    } buf;
};

#define serial_to_async(s)  container_of(s, struct serial_async, serial)

extern const struct vtbl_error_reporting_dev serial_async_erd_ops;

#define SERIAL_ASYNC_ERD_INIT(min_priority, mod)                \
    {                                                           \
    .serial = SERIAL_ERD_INIT(min_priority, mod),               \
    .serial.erd = ERD_INIT(min_priority, serial_async_erd_ops), \
    .buf = {0}                                                  \
    }
#define SERIAL_ASYNC_INIT(mod)  \
    SERIAL_ASYNC_ERD_INIT(REP_DEBUG, mod)

#define SERIAL_ASYNC_ERD(name, min_priority, mod)   \
    struct serial_async name = SERIAL_ASYNC_ERD_INIT(min_priority, mod)
#define SERIAL_ASYNC(name, mod) \
    SERIAL_ASYNC_ERD(name, REP_DEBUG, mod)

void
serial_async_tx(struct serial_async *s, const void *src, size_t n);

void PRINTF(2, 3)
serial_async_printf(struct serial_async *s, const char *fmt, ...);

static ALWAYSINLINE void
serial_async_puts(struct serial_async *s, const char *str)
{
    serial_async_tx(s, str, strlen(str));
}

void
serial_async_rx(struct serial_async *s, void *dst, size_t n);

/* this command won't return until all data in the async
 * tx buffer is sent
 */
void
serial_async_tx_flush(struct serial_async *s);

#endif

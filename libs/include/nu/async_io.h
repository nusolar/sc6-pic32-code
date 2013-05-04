#ifndef NU_ASYNC_IO_H
#define NU_ASYNC_IO_H 1

#include "nu/circ_buf.h"
#include "nu/types.h"
#include "nu/utility/arith.h"
#include <sys/types.h>

struct nu_async_io {
    struct circ_buf tx_buf;
    int tx_buf_size;
    struct circ_buf rx_buf;
    int rx_buf_size;
    const struct nu_vtbl_async_io *op;
};
#define NU_ASYNC_IO_INIT(tx_buf, tx_buf_siz, rx_buf, rx_buf_siz, op) \
    { \
    CIRC_BUF_INIT(tx_buf, 0, 0), \
    round_down(tx_buf_siz, 2), \
    CIRC_BUF_INIT(rx_buf, 0, 0), \
    round_down(rx_buf_siz, 2), \
    (op) \
    }

/**
 * Virtual function table
 */
struct nu_vtbl_async_io {
    /* positive return indicates # of bytes that were added/removed to/from hardware tx/rx buffer
     * return of 0 indicates that no additional bytes can be added/removed to/from hardware tx/rx buffer
     * negative return indicates error
     */
    ssize_t (*tx_callback) (const void *src, size_t n);
    ssize_t (*rx_callback) (void *dst, size_t n);
};

/* to be done outside ISRs */

/* returns # of bytes enqueued */
size_t
nu_async_io_tx_enqueue(struct nu_async_io *a, const void *src, size_t n, bool overrun);

/* returns # of bytes dequeued */
size_t
nu_async_io_rx_dequeue(struct nu_async_io *a, void *dst, size_t n);

/* to be done inside ISRs; negative return indicates error */

ssize_t
nu_async_io_tx(struct nu_async_io *a);

ssize_t
nu_async_io_rx(struct nu_async_io *a);

#endif

#include "async_io.h"

const char *default_rx_delims = "\r\n";

s32 async_io_setup(struct async_io *a) { a->op->setup(a); }

s32
async_io_tx(struct async_io *a, const void *src, size_t n, bool overrun, bool *overrun_out)
{
    struct circ_buf *tx_buf = &(a->tx_buf);
    size_t ui;
    for (ui = 0; ui < n; ++ui) {
        tx_buf->buf[tx_buf->head] = ((const u8 *)src)[ui];
        tx_buf->head = (tx_buf->head + 1) & (a->tx_buf_size - 1);
        if (CIRC_SPACE(tx_buf->head, tx_buf->tail, a->tx_buf_size) < 1) {
            if (!overrun)
                break;
            if (overrun_out)
                *overrun_out = true;
            tx_buf->tail = (tx_buf->head - 1) & (a->tx_buf_size - 1);
        }
    }
    return ui;
}

s32
async_io_rx(struct async_io *a, void *dst, size_t n)
{
    return 0;
}
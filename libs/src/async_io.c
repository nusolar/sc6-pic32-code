#include "async_io.h"

size_t
async_io_tx_enqueue(struct nu_async_io *a, const void *src, size_t n, bool overrun)
{
    struct circ_buf *tx_buf = &(a->tx_buf);
    size_t ui;
    for (ui = 0; ui < n; ++ui) {
        tx_buf->buf[tx_buf->head] = *((const u8 *)src + ui);
        tx_buf->head = (tx_buf->head + 1) & (a->tx_buf_size - 1);
        if (CIRC_SPACE(tx_buf->head, tx_buf->tail, a->tx_buf_size) < 1) {
            if (!overrun)
                break;
            tx_buf->tail = (tx_buf->head - 1) & (a->tx_buf_size - 1);
        }
    }
    return ui;
}

size_t
nu_async_io_rx_dequeue(struct nu_async_io *a, void *dst, size_t n)
{
    return 0;
}

ssize_t
nu_async_io_tx(struct nu_async_io *a)
{
    return 0;
}

ssize_t
nu_async_io_rx(struct nu_async_io *a)
{
    return 0;
}

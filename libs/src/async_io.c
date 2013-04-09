#include "async_io.h"
#include "compiler.h"

static void
producer_enter(struct nu_async_io *a, struct circ_buf *b)
{
    (void)a;
    (void)b;
}

static void
producer_exit(struct nu_async_io *a, struct circ_buf *b)
{
    (void)a;
    (void)b;
}

static void
consumer_enter(struct nu_async_io *a, struct circ_buf *b)
{
    (void)a;
    (void)b;
}

static void
consumer_exit(struct nu_async_io *a, struct circ_buf *b)
{
    (void)a;
    (void)b;
}

size_t
nu_async_io_tx_enqueue(struct nu_async_io *a, const void *src, size_t n, bool overrun)
{
    struct circ_buf *tx_buf = &(a->tx_buf);
    size_t ui;
    producer_enter(a, tx_buf);
    for (ui = 0; ui < n; ++ui) {
        tx_buf->buf[tx_buf->head] = *((const char *)src + ui);
        tx_buf->head = (tx_buf->head + 1) & (a->tx_buf_size - 1);
        if (CIRC_SPACE(tx_buf->head, tx_buf->tail, a->tx_buf_size) < 1) {
            if (!overrun)
                break;
            tx_buf->tail = (tx_buf->head - 1) & (a->tx_buf_size - 1);
        }
    }
    producer_exit(a, tx_buf);
    return ui;
}

size_t
nu_async_io_rx_dequeue(struct nu_async_io *a, void *dst, size_t n)
{
    (void)a;
    (void)dst;
    (void)n;
    consumer_enter(a, &(a->rx_buf));
    consumer_exit(a, &(a->rx_buf));
    return 0;
}

ssize_t
nu_async_io_tx(struct nu_async_io *a)
{
    struct circ_buf *buf = &(a->tx_buf);
    int cnt_to_end;
    ssize_t sent_total = 0;

    if (!CIRC_CNT(buf->head, buf->tail, a->tx_buf_size))
        return 0;

    consumer_enter(a, &(a->tx_buf));

    for (cnt_to_end = CIRC_CNT_TO_END2(buf->head, buf->tail, a->tx_buf_size);
            cnt_to_end;
            cnt_to_end = CIRC_CNT_TO_END2(buf->head, buf->tail, a->tx_buf_size)) {
        ssize_t tx_result = a->op->tx_callback(buf->buf + buf->tail, (size_t)cnt_to_end);

        if (tx_result < 0)
            return tx_result;

        sent_total += tx_result;
        buf->tail = (buf->tail + tx_result) & (a->tx_buf_size - 1);
        if (tx_result <= cnt_to_end)
            break;
    }

    consumer_exit(a, &(a->tx_buf));

    return sent_total;
}

ssize_t
nu_async_io_rx(struct nu_async_io *a)
{
    (void)a;
    producer_enter(a, &(a->rx_buf));
    producer_exit(a, &(a->rx_buf));
    return 0;
}

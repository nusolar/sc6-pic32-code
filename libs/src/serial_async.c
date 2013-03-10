#include "serial_async.h"

const char *default_delims = "\r\n";

COLD static void
serial_async_report(struct error_reporting_dev *e,
    const char *file, const char *func, u32 line, const char *expr,
    UNUSED enum report_priority priority, s32 err, const char *err_s,
    const char *fmtd_msg)
{
    struct serial *s = erd_to_serial(e);
    struct serial_async *sa = serial_to_async(s);
    size_t totalwritten;

    const char *fmt =   "%s:%s:%d:%s\r\n"
                        "\tERR %d(%s)\r\n"
                        "\t%s\r\n\r\n";

    char *buf;
    size_t n;
    n = snprintf(NULL, 0, fmt, file, func, line, expr, err, err_s, fmtd_msg);
    buf = alloca(n);
    sprintf(buf, fmt, file, func, line, expr, err, err_s, fmtd_msg);

    for (totalwritten = 0;
            totalwritten < n;
            totalwritten += serial_async_tx(sa, buf+totalwritten, n-totalwritten, false, NULL))
        Nop();
}

const struct vtbl_error_reporting_dev serial_async_erd_ops = {
    .report             = serial_async_report,
    .reset_err_state    = NULL
};

/* @TODO should not overrun itself */
s32
serial_async_tx(struct serial_async *s, const void *src, size_t n, bool overrun, bool *overrun_out)
{
    struct circ_buf *tx_buf = s->tx_buf;
    size_t ui;
    for (ui = 0; ui < n; ++ui) {
        s->tx_data[tx_buf->head] = ((const u8 *)src)[ui];
        tx_buf->head = (tx_buf->head + 1) & (s->tx_buf_size - 1);
        if (CIRC_SPACE(tx_buf->head, tx_buf->tail, s->tx_buf_size) < 1) {
            if (!overrun)
                break;
            if (overrun_out)
                *overrun_out = true;
            tx_buf->tail = (tx_buf->head - 1) & (s->tx_buf_size - 1);
        }
    }
    return ui;
}

void PRINTF(4,5)
serial_async_printf(struct serial_async *s, bool overrun, bool *overrun_out, const char *fmt, ...)
{
    char *buf;
    size_t n;
    va_list fmtargs;
    va_start(fmtargs, fmt);
    n = vsnprintf(NULL, 0, fmt, fmtargs);
    buf = alloca(n);
    vsprintf(buf, fmt, fmtargs);
    va_end(fmtargs);
    serial_async_tx(s, buf, n, overrun, overrun_out);
}

#if 0
static struct serial *u1 = NULL;

static void __ISR(_UART1_VECTOR)
IntU1Handler(void)
{

}

void __attribute__ ((interrupt,no_auto_psv)) _U2TXInterrupt(void)
{
//the UART2 Tx Buffer is empty (!UART_TX_BUF_FULL()), fill it
//Only if data exists in data buffer (!isTxBufEmpty())
while(!isTxBufEmpty()&& !UART_TX_BUF_FULL())    {
    if(BT_CONNECTED)
    {   //Transmit next byte of data
        U2TXREG = 0xFF & (unsigned int)txbuf[txReadPtr];
        txReadPtr = (txReadPtr + 1) % TX_BUFFER_SIZE;
    }else{
        break;
    }
}
IFS1bits.U2TXIF = 0;
}

#endif
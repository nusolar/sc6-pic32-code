#include <alloca.h>
#include "errorcodes.h"
#include "nu32.h"
#include "serial.h"
#include "utility.h"
#include "wdt.h"

COLD static void
serial_report(struct error_reporting_dev *e,
    const char *file, const char *func, u32 line, const char *expr,
    UNUSED enum report_priority priority, s32 err, const char *err_s,
    const char *fmtd_msg)
{
    struct serial *s = erd_to_serial(e);

    /*
    if (s->is_async) {
        struct serial_async *as = serial_to_async(s);
        if (as->erd_async) {
            serial_printf_async()
        }
    }
     */

    serial_printf(s,
        "%s:%s:%d:%s\r\n"
        "\tERR %d(%s)\r\n"
        "\t%s\r\n\r\n",
        file, func, line, expr,
        err, err_s,
        fmtd_msg);
}

const struct vtbl_error_reporting_dev serial_erd_ops = {
    .report         = &serial_report,
    .reset_err_state= NULL,
};

void
serial_setup(struct serial *s, u32 baud, enum module_interrupt use_interrupt,
        INT_PRIORITY int_priority, UART_FIFO_MODE interrupt_modes,
        UART_LINE_CONTROL_MODE line_control_modes, UART_CONFIGURATION uart_config,
        UART_ENABLE_MODE enable_modes)
{
    INT_VECTOR int_vect;
    INT_SOURCE int_src;

    UARTConfigure(s->module, uart_config);
    UARTSetFifoMode(s->module, interrupt_modes);
    UARTSetLineControl(s->module, line_control_modes);
    UARTSetDataRate(s->module, HZ, baud);
    UARTEnable(s->module, UART_ENABLE_FLAGS(enable_modes));

    if (USE_UART_INTERRUPT == use_interrupt) {
        switch(s->module) {
            case UART1: 
                int_vect = INT_UART_1_VECTOR;
                int_src = INT_U1RX;
                break;
            case UART2:
                int_vect = INT_UART_2_VECTOR;
                int_src = INT_U2RX;
                break;
            case UART3:
                int_vect = INT_UART_3_VECTOR;
                int_src = INT_U3RX;
                break;
            case UART4:
                int_vect = INT_UART_4_VECTOR;
                int_src = INT_U4RX;
                break;
            case UART5:
                int_vect = INT_UART_5_VECTOR;
                int_src = INT_U5RX;
                break;
            case UART6:
                int_vect = INT_UART_6_VECTOR;
                int_src = INT_U6RX;
                break;
            case UART_NUMBER_OF_MODULES: 
            default:
                break;
        }
        INTEnable(int_src, INT_ENABLED);
        INTSetVectorPriority(int_vect, int_priority);
        INTSetVectorSubPriority(int_vect, INT_SUB_PRIORITY_LEVEL_0);
    }
}

void
serial_tx(const struct serial *s, const void *src, size_t n)
{
    size_t ui;

    for (ui = 0; ui < n; ++ui) {
        clear_wdt();
        while (!UARTTransmitterIsReady(s->module))
            ;   /* do nothing */
        UARTSendDataByte(s->module, ((const BYTE *)src)[ui]);
    }

    while (!UARTTransmissionHasCompleted(s->module))
        ;   /* do nothing */
}

void PRINTF(2,3)
serial_printf(const struct serial *s, const char *fmt, ...)
{
    va_list fmtargs;
    char *buf;
    size_t n;

    if (unlikely(!fmt))
        return;

    va_start(fmtargs, fmt);
    n = vsnprintf(NULL, 0, fmt, fmtargs);
    buf = alloca(n);
    vsprintf(buf, fmt, fmtargs);
    va_end(fmtargs);

    serial_tx(s, buf, n);
}

s32
serial_rx(struct serial *s, void *dst, size_t n)
{
    s32 err = 0;
    BOOL rxMsgSuccess = FALSE;

    while (UARTReceivedDataIsAvailable(s->module)) {
        s->rxBuf[s->currentBufPos] = UARTGetDataByte(s->module);
        if (s->currentBufPos == ARRAY_SIZE(self->rxBuf)-1)
            s->rxBufFull = TRUE;

        if (matchesDelim(self, self->rxBuf[s->currentBufPos])) {
            rxMsgSuccess = TRUE;

            err = bufDelimCpy(s, s->currentBufPos, dst, len);

            reset_rx_buf(self);
            break;
        }

        self->currentBufPos = (self->currentBufPos + 1) % ARRAY_SIZE(self->rxBuf);
    }

    if (rxMsgSuccess == FALSE)
        err = -ENODATA;

    return err;
}

static int32_t
bufDelimCpy (const struct serial *self, uint32_t delimPos, void *dst, size_t len)
{
    uint32_t ui, uj;

    if (self == NULL || dst == NULL)
        return -ENULPTR;

    ui = (self->rxBufFull == TRUE) ?
        (delimPos+1)%ARRAY_SIZE(self->rxBuf) : 0;
    uj = 0;
    for ( ; ui != delimPos; ui = (ui+1)%ARRAY_SIZE(self->rxBuf)) {
        if (self->rxBuf[ui]) {
            if (uj < len)
                ((BYTE *)dst)[uj++] = self->rxBuf[ui];
            else
                return -ETRUNCATED;
        }
    }


    return 0;
}

static int32_t
matchesDelim(const struct serial *self, BYTE val)
{
    size_t i;
    for (i = 0; i < self->numDelims; i++)
        if (val == self->delims[i])
            return 1;

    return 0;
}

static int32_t
flushRxBuf (struct serial *self, void *dst, size_t len)
{
    if (self == NULL)
        return -ENULPTR;

    if (dst != NULL && len >= 1)
        memcpy(dst, self->rxBuf, MIN(sizeof(self->rxBuf), len));
    
    return reset_rx_buf(self);
}

static INLINE void
reset_rx_buf(struct serial *s)
{
    memset(s->rx_buf, 0, sizeof(s->rx_buf));
    s->buf_pos = 0;
    s->rx_buf_full = false;
}
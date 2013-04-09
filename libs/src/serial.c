#include "nu/serial.h"
#include "nu/errorcodes.h"
#include "nu/nu32.h"
#include "nu/utility.h"
#include "nu/wdt.h"
#include <alloca.h>

static COLD void
nu_serial_report(struct nu_error_reporting_dev *e,
    const char *file, const char *func, u32 line, const char *expr,
    UNUSED enum nu_report_priority priority, s32 err, const char *err_s,
    const char *fmtd_msg)
{
    struct nu_serial *s = nu_erd_to_serial(e);

    nu_serial_printf(s,
        "%s:%s:%d:%s\r\n"
        "\tERR %d(%s)\r\n"
        "\t%s\r\n\r\n",
        file, func, line, expr,
        err, err_s,
        fmtd_msg);
}

const struct nu_vtbl_error_reporting_dev nu_serial_erd_ops = {
    &nu_serial_report,
    NULL
};

void
nu_serial_setup(struct nu_serial *s, u32 baud, enum nu_serial_module_interrupt use_interrupt,
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

    if (NU_USE_UART_INTERRUPT == use_interrupt) {
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
nu_serial_tx(const struct nu_serial *s, const void *src, size_t n)
{
    size_t ui;
    for (ui = 0; ui < n; ++ui) {
        clear_wdt();
        while (!UARTTransmitterIsReady(s->module))
            Nop();
        UARTSendDataByte(s->module, ((const BYTE *)src)[ui]);
    }
}

void
nu_serial_printf(const struct nu_serial *s, const char *fmt, ...)
{
    char *buf;
    size_t n;
    va_list fmtargs;
    va_start(fmtargs, fmt);
    n = vsnprintf(NULL, 0, fmt, fmtargs);
    buf = alloca(n);
    vsprintf(buf, fmt, fmtargs);
    va_end(fmtargs);
    nu_serial_tx(s, buf, n);
}

void
nu_serial_rx(const struct nu_serial *s, void *dst, size_t n)
{
    size_t ui;
    for (ui = 0; ui < n && UARTReceivedDataIsAvailable(s->module); ++ui)
        ((u8 *)dst)[ui] = UARTGetDataByte(s->module);
}

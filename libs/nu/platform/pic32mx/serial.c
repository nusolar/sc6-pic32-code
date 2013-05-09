#include "nu/param.h"
#include "nu/platform/serial.h"

void
nu_serial_platform_setup(struct nu_serial_platform *platform, u32 baud,
	const struct nu_serial_platform_setup_args *arg)
{
    UARTConfigure(platform->module, arg->uart_config);
    UARTSetFifoMode(platform->module, arg->interrupt_modes);
    UARTSetLineControl(platform->module, arg->line_control_modes);
    UARTSetDataRate(platform->module, NU_HZ, baud);
    UARTEnable(platform->module, (UART_ENABLE_MODE) UART_ENABLE_FLAGS(arg->enable_modes));

    if (NU_USE_UART_INTERRUPT == arg->use_interrupt) {
        INT_VECTOR int_vect;
        INT_SOURCE int_src;
        switch(platform->module) {
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
            return;
        }
        INTEnable(int_src, INT_ENABLED);
        INTSetVectorPriority(int_vect, arg->int_priority);
        INTSetVectorSubPriority(int_vect, INT_SUB_PRIORITY_LEVEL_0);
    }
}

s32
nu_serial_platform_putchar(const struct nu_serial_platform *p, s32 c)
{
    while (!UARTTransmitterIsReady(p->module))
        Nop();
    UARTSendDataByte(p->module, (u8)c);
    return c;
}

s32
nu_serial_platform_getchar(const struct nu_serial_platform *p)
{
    return UARTReceivedDataIsAvailable(p->module) ?
        UARTGetDataByte(p->module) : EOF;
}

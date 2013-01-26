#include <peripheral/adc10.h>
#include <peripheral/cvref.h>
#include <peripheral/eth.h>
#include <peripheral/pmp.h>
#include <peripheral/system.h>
#include "nu32.h"
#include "nu_types.h"

WEAK CONSTRUCTOR() void
nu32_setup(void)
{
    SYSTEMConfig(HZ, SYS_CFG_ALL);
    /* configure for multi-vectored mode */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    /* enable multi-vector interrupts */
    INTEnableSystemMultiVectoredInt();
    /* disable JTAG to get A4 and A5 back */
    DDPCONbits.JTAGEN = 0;
    /* disable comparator in attempt to get A9 and A10 back */
    CVREFOpen(CVREF_DISABLE);
    /* disable ADC in attempt to get A9 and A10 back...possibly only need the
     * second config to do this
     */
    OpenADC10(ADC_MODULE_OFF, ADC_VREF_AVDD_AVSS, 0, 0, 0);
    /* disable ethernet controller */
    EthEnable(0);
    /* disable parallel port */
    mPMPDisable();

    /* Configure LED outputs */
    led_setup(&nu32_led0);
    led_setup(&nu32_led1);

    /* Configure input for switch */
    pin_set_digital_in(&nu32_user);
}

void
nu32_setup_leds(void)
{
    led_setup(&nu32_led0);
    led_setup(&nu32_led1);
}

#if 0 /* for posterity... */

void __attribute__((deprecated))
initSerialNU32v2(uint32_t serialBaudRate)
{
    uint32_t pbClk = sys_clk_hz/((uint32_t)(1 << OSCCONbits.PBDIV));
    
    /* Configure the system performance */
    /* pbClk = SYSTEMConfigPerformance(sysFreqHz); */ /* this line is redundant */

    UARTConfigure(NU32_UART_MODULE, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(NU32_UART_MODULE, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(NU32_UART_MODULE, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    
    UARTSetDataRate(NU32_UART_MODULE, (UINT32)pbClk, serialBaudRate);
    UARTEnable(NU32_UART_MODULE, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    /* Configure UART3 RX Interrupt */
    INTEnable(INT_U3RX, INT_ENABLED);
    INTSetVectorPriority(INT_UART_3_VECTOR, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_UART_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
}

void __attribute__((deprecated))
WriteString(UART_MODULE id, const char *string)
{
    for ( ; *string != '\0'; string++) {
        while(!UARTTransmitterIsReady(id))
            ;     /* do nothing */
        UARTSendDataByte(id, (const BYTE)*string);
        while(!UARTTransmissionHasCompleted(id))
            ;     /* do nothing */
    }
}

void __attribute__((deprecated))
PutCharacter(UART_MODULE id, char character)
{
    while(!UARTTransmitterIsReady(id))
        ;   /* do nothing */
    UARTSendDataByte(id, (BYTE)character);
    while(!UARTTransmissionHasCompleted(id))
        ;   /* do nothing */
}

#endif

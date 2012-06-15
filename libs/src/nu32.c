#include "../include/nu32.h"

uint32_t sys_clk_hz = 80000000; /* 80 MHz */

int32_t
nu32_init(uint32_t sysClkHz)
{
    sys_clk_hz = sysClkHz;

    SYSTEMConfig(sys_clk_hz, SYS_CFG_ALL);
	
    /* configure for multi-vectored mode */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    /* enable multi-vector interrupts */
    INTEnableSystemMultiVectoredInt();
	
    /* Configure LED outputs */
    PORTSetPinsDigitalOut(NU32_LED0_PORT, NU32_LED0_PIN);
    PORTSetPinsDigitalOut(NU32_LED1_PORT, NU32_LED1_PIN);

    /* Configure input for switch */
    PORTSetPinsDigitalIn(NU32_SWITCH_PORT, NU32_SWITCH_PIN);

    return 0;
}

void
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

void
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

void
PutCharacter(UART_MODULE id, char character)
{
    while(!UARTTransmitterIsReady(id))
        ;   /* do nothing */
    UARTSendDataByte(id, (BYTE)character);
    while(!UARTTransmissionHasCompleted(id))
        ;   /* do nothing */
}

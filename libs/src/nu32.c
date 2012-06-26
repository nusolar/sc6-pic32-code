#include "../include/nu32.h"

struct led nu32_led0, *nu32_led0p = NULL;
struct led nu32_led1, *nu32_led1p = NULL;

struct serial nu32_serial, *nu32_serp = &nu32_serial;

uint32_t sys_clk_hz = 80000000; /* 80 MHz */

#define busywait(n) do{uint32_t _ui; for (_ui = 0; _ui < n; ++_ui);}while(0)

int32_t
nu32_init(uint32_t sysClkHz)
{
    sys_clk_hz = sysClkHz;

    SYSTEMConfig(sys_clk_hz, SYS_CFG_ALL);

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
    PORTSetPinsDigitalOut(NU32_LED0_PORT, NU32_LED0_PIN);
    PORTSetPinsDigitalOut(NU32_LED1_PORT, NU32_LED1_PIN);

    /* Configure input for switch */
    PORTSetPinsDigitalIn(NU32_USER_PORT, NU32_USER_PIN);

    return 0;
}

int32_t
nu32_init_leds(void)
{
    int32_t ret1, ret2;

    if ((ret1 = led_new(&nu32_led0, NU32_LED0_PORT, NU32_LED0_PIN)) >= 0)
        nu32_led0p = &nu32_led0;
    if ((ret2 = led_new(nu32_led1p, NU32_LED1_PORT, NU32_LED1_PIN)) >= 0)
        nu32_led1p = &nu32_led1;

    return (ret1 < 0) ? ret1 : ret2;
}

int32_t
nu32_init_serial(uint32_t baud)
{
    int32_t ret;
    BYTE delims[2] = {'\r', '\n'};
    if ((ret = serial_new(&nu32_serial, NU32_UART_MODULE, baud, NO_UART_INTERRUPT,
            INT_PRIORITY_DISABLED, 0, UART_DATA_SIZE_8_BITS, 0,
            UART_ENABLE|UART_TX|UART_RX, delims, sizeof(delims))) >= 0)
        nu32_serp = &nu32_serial;

    return ret;
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

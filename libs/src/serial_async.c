#include "serial_async.h"

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

#include <plib.h>
#include <stdio.h>
#include "../libs/include/Nokia5110.h"
#include "../libs/include/NUCAN.h"
#include "../libs/include/NU32v2.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

BYTE CAN1MessageFifoArea[2 * 8 * CAN_TX_RX_MESSAGE_SIZE_BYTES];
BYTE CAN2MessageFifoArea[2 * 8 * CAN_TX_RX_MESSAGE_SIZE_BYTES];

int
main (void)
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);

    // configure for multi-vectored mode
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    // initialize serial port and interrupt
    initSerialNU32v2();
    initLEDs(); 	// set LED pins as outputs
    LED0 = 1;
    LED1 = 0;

    // init and clear the LCD
    LcdInitialize();
    LcdClear();

    // write to the PC
    WriteString(UART3, "\r\nHello World!\r\n");

    NUCANInitializeModule(CAN1, FALSE);

    while(1)
        ; // do nothing

    return 0;
}

void __ISR(_CAN_1_VECTOR, ipl4) Can1IntHandler(void)
{
    /* Check if the source of the interrupt is
	 * RX_EVENT. This is redundant since only this
     * event is enabled in this example but
     * this shows one scheme for handling events. */

	if((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0)
	{

		/* Within this, you can check which channel caused the
		 * event by using the CANGetModuleEvent() function
         * which returns a code representing the highest priority
         * pending event. */

		if(CANGetPendingEventCode(CAN1) == CAN_CHANNEL1_EVENT)
		{
			/* This means that channel 1 caused the event.
			 * The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You
			 * could either read the channel in the ISR
			 * to clear the event condition or as done
			 * here, disable the event source, and set
			 * an application flag to indicate that a message
			 * has been received. The event can be
			 * enabled by the application when it has processed
			 * one message.
			 *
			 * Note that leaving the event enabled would
			 * cause the CPU to keep executing the ISR since
			 * the CAN_RX_CHANNEL_NOT_EMPTY event is persistent (unless
			 * the not empty condition is cleared.)
			 * */

            CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            INTClearFlag(INT_CAN1);

            CANRxMessageBuffer *rxMsg;
        	rxMsg = CANGetRxMessage(CAN1,CAN_CHANNEL1);

        	char buff[50];
        	sprintf(buff, "ID %d     ", (unsigned int)(rxMsg->msgSID.SID));

        	CANUpdateChannel(CAN1, CAN_CHANNEL1);
        	CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);

        	gotoXY(0,0);
        	LcdString(buff);
		}
	} else {
	    INTClearFlag(INT_CAN1);
	}
}

// UART3 interrupt handler, priority level 2
void __ISR(_UART_3_VECTOR, ipl2) IntUart3Handler(void) {
  // Is this an RX interrupt?
  if(INTGetFlag(INT_SOURCE_UART_RX(UART3))){
    char data = UARTGetDataByte(UART3);

    // Echo what we just received
    PutCharacter(UART3, data);

    // do something with the data
    if(data =='a') {
      // do something
      LcdClear();
    }

    // Clear the RX interrupt Flag
    INTClearFlag(INT_SOURCE_UART_RX(UART3));
  }

  // We don't care about TX interrupt
  if(INTGetFlag(INT_SOURCE_UART_TX(UART3))) {
    INTClearFlag(INT_SOURCE_UART_TX(UART3));
  }
}

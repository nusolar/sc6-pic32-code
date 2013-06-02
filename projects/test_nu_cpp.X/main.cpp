/* 
 * File:   main.cpp
 * Author: alex
 *
 * Created on May 16, 2013, 11:58 PM
 */

#include "nu/platform/common_pragmas.h"
#include "nupp/nu32.hpp"
#include "nupp/timer.hpp"
#include "nu/compiler.h"
#include "nupp/nokia5110.hpp"
#include <plib.h>

extern "C" {
	// declared static in case exception condition would prevent
	// auto variable being created
	static enum exceptions {
		EXCEP_IRQ = 0,          // interrupt
		EXCEP_AdEL = 4,         // address error exception (load or ifetch)
		EXCEP_AdES,             // address error exception (store)
		EXCEP_IBE,              // bus error (ifetch)
		EXCEP_DBE,              // bus error (load/store)
		EXCEP_Sys,              // syscall
		EXCEP_Bp,               // breakpoint
		EXCEP_RI,               // reserved instruction
		EXCEP_CpU,              // coprocessor unusable
		EXCEP_Overflow,         // arithmetic overflow
		EXCEP_Trap,             // trap (possible divide by zero)
		EXCEP_IS1 = 16,         // implementation specfic 1
		EXCEP_CEU,              // CorExtend Unuseable
		EXCEP_C2E               // coprocessor 2
	} _excep_code;



//	static unsigned int _epc_code;
	static unsigned int _excep_addr;

	// this function overrides the normal _weak_ generic handler
	void _general_exception_handler(void)
	{
		asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
		asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

		_excep_code = (exceptions) ((_excep_code & 0x0000007C) >> 2);

		while (1) {
			Nop();
			// Examine _excep_code to identify the type of exception
			// Examine _excep_addr to find the address that caused the exception
		}
	}
}

#define SYS_FREQ 				(80000000L)
#define TIMER1_PERIOD 	0x1F40	/* For a 0.1 millisecond interval	*/
#define ONE_SECOND_MSEC	0x2710

extern "C" {
	static volatile BOOL isCAN1MsgReceived = FALSE;
	static volatile BOOL isCAN2MsgReceived = FALSE;
	static BYTE	LED5Status = 0;
	static BYTE	LED6Status = 0;
	static BYTE CAN1MessageFifoArea[2 * 8 * 16] = {0};


	void can_init() {
		CAN_BIT_CONFIG canBitConfig;
		UINT baudPrescalar;
		CANEnableModule(CAN1,TRUE);
		CANSetOperatingMode(CAN1, CAN_CONFIGURATION);
		while(CANGetOperatingMode(CAN1) != CAN_CONFIGURATION);
		canBitConfig.phaseSeg2Tq            = CAN_BIT_3TQ;
		canBitConfig.phaseSeg1Tq            = CAN_BIT_3TQ;
		canBitConfig.propagationSegTq       = CAN_BIT_3TQ;
		canBitConfig.phaseSeg2TimeSelect    = TRUE;
		canBitConfig.sample3Time            = TRUE;
		canBitConfig.syncJumpWidth          = CAN_BIT_2TQ;
		CANSetSpeed(CAN1,&canBitConfig, 80000000UL, 1000000);
		CANAssignMemoryBuffer(CAN1, CAN1MessageFifoArea, (2 * 8 * 16));
		CANConfigureChannelForTx(CAN1, CAN_CHANNEL0, 8, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
		CANConfigureChannelForRx(CAN1, CAN_CHANNEL1, 8, CAN_RX_FULL_RECEIVE);
		CANConfigureFilter      (CAN1, CAN_FILTER0, 0x401, CAN_SID);
		CANConfigureFilterMask  (CAN1, CAN_FILTER_MASK0, 0x7FF, CAN_SID, CAN_FILTER_MASK_IDE_TYPE);
		CANLinkFilterToChannel  (CAN1, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
		CANEnableFilter         (CAN1, CAN_FILTER0, TRUE);
		CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
		CANEnableModuleEvent (CAN1, CAN_RX_EVENT, TRUE);
		INTSetVectorPriority(INT_CAN_1_VECTOR, INT_PRIORITY_LEVEL_4);
		INTSetVectorSubPriority(INT_CAN_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
		INTEnable(INT_CAN1, INT_ENABLED);
		CANSetOperatingMode(CAN1, CAN_NORMAL_OPERATION);
		while(CANGetOperatingMode(CAN1) != CAN_NORMAL_OPERATION);
	}

	void __attribute__((vector(46), interrupt(ipl4), nomips16)) CAN1InterruptHandler(void) {
		if ((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0) {
			if(CANGetPendingEventCode(CAN1) == CAN_CHANNEL1_EVENT) {
				CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
				isCAN1MsgReceived = TRUE;
			}
		}
		INTClearFlag(INT_CAN1);
	}

	void Timer1Init(void){
		/* This function will intialize the Timer 1
		 * for basic timer operation. It will enable
		 * timer interrupt. The one second flag is
		 * initialized and the millisecond counter is
		 * initialized. */

		T1CON = 0x0;			/* Basic Timer Operation				*/
		PR1 = TIMER1_PERIOD;	/* Timer period in TimerFunctions.h 	*/

		IFS0CLR = 0x10;			/* Clear interrupt flag and enable		*/
		IEC0SET = 0x10;			/* timer 1 interrupt. 					*/
		IPC1bits.T1IP = 4;		/* Timer 1 priority is 4				*/

//		oneSecondUp = FALSE;	/* Intialize the one second flag		*/
//		msecCounter = 0;		/* and the millisecond counter.			*/s

		T1CONSET = 0x8000;		/* Start the timer.						*/

	}
}

using namespace nu;
int main() {
	Nu32 nu32(Nu32::V1);
	Nokia5110 lcd(Pin(Pin::G, 9), SPI_CHANNEL2, Pin(Pin::A, 9), Pin(Pin::E, 9));
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
	can_init();

	nu32.led1.on(); // NOT LED 1
	nu32.led2.on();

	while (true) {
		nu32.led2.off();
		lcd.lcd_clear();
		lcd << "Test" << end;
		lcd.goto_xy(0,1);
		lcd << timer::s() << end;


		CANRxMessageBuffer *message = CANGetRxMessage(CAN1,CAN_CHANNEL1);
		lcd.goto_xy(0,2);
		lcd << (isCAN1MsgReceived) << " " << (message != NULL) << end;
		if (isCAN1MsgReceived != FALSE || message != NULL) {
			
			unsigned int sid = message->msgSID.SID; // THIS LINE FAILS
			if (sid < 0x400) {
				nu32.led2.off();
			} else {
				nu32.led2.on();
			}

			CANUpdateChannel(CAN1, CAN_CHANNEL1);
			CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);

			nu32.led2.on();
			lcd.goto_xy(0,3);
			lcd << sid << ":" << end;
			timer::delay_s<1>();
		}
		isCAN1MsgReceived = FALSE;
	}
	return 0;
}

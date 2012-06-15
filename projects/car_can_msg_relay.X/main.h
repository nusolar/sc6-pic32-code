#ifndef __NU_CAN_RELAY_MSG_MAIN_H
#define __NU_CAN_RELAY_MSG_MAIN_H

#include "nu32.h"
#include "can.h"
#include "serial.h"

/* config pragmas needed mostly for PICKit3 */
#pragma config ICESEL   = ICS_PGx2
#pragma config FPLLMUL  = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV   = DIV_1
#pragma config POSCMOD  = XT, FNOSC = PRIPLL

/* disable WDT (we'll control it in software) */
#pragma config FWDTEN   = OFF

/* 2.048s WDT timeout (1:2048 postscaler) */
#pragma config WDTPS    = PS2048

const uint32_t      SYS_CLK_HZ                  = 80000000;

const CAN_MODULE    COMMON_CAN_MOD              = CAN1;
const CAN_CHANNEL   COMMON_CAN_TX_CHN           = CAN_CHANNEL0;
const CAN_CHANNEL   COMMON_CAN_RX_CHN           = CAN_CHANNEL1;



#endif
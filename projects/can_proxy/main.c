//t[can id][data length][data]
//eg. t40081234567890123456
#include "../libs/include/NUCAN.h"
#include "../libs/include/NU32v2.h"
#include "../libs/include/Nokia5110.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

int main(void)
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
    
    initSerialNU32v2();
    
    NUCANInitializeModule(CAN1, FALSE);
    
    while (1){
        unsigned int id, len;
        BYTE rxData[8];
        if (!NUCANRx(CAN1, &id, rxData, &len)) {
            const int BUFFSIZE = 50;
            char buff[BUFFSIZE];
    
            snprintf(buff,BUFFSIZE,"t%03X%X",id,len);
            int i;
            for(i =0; i<len;i++)
            {
                snprintf(buff,BUFFSIZE, "%s%03X", buff,rxData[i]);
            }
            snprintf(buff,BUFFSIZE,"%s\r\n",buff);
            sprintf(RS232_Out_Buffer,"%s", buff);
            WriteString(UART3, RS232_Out_Buffer);
            char str[BUFFSIZE];
            sprintf(str, "%s",buff);
            LcdString(str);
        
        }         
	}
}

#include <plib.h>
#include <stdio.h>
#include <string.h>
#include "../libs/include/Nokia5110.h"
#include "../libs/include/Nokia5110_new.h"
#include "../libs/include/NU32v2.h"

#pragma config ICESEL = ICS_PGx2
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = XT, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

#define NVM_PROGRAM_PAGE 0xbd008000
#define NVM_PAGE_SIZE    4096

unsigned int pagebuff[1024];
unsigned int databuff[128];

int
main(void)
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

    memset(databuff, 0, sizeof(databuff));

    memcpy(databuff, (void *)NVM_PROGRAM_PAGE, sizeof(databuff));
    gotoXY(0,0);
    LcdString((char *)databuff);

    memset(databuff, 0, sizeof(databuff));
    sprintf((char *)databuff, "HALLO, THAR!");

    // Erase second page of Program Flash
    NVMErasePage((void *)NVM_PROGRAM_PAGE);

    // Write 128 words starting at Row Address NVM_PROGRAM_PAGE
    NVMWriteRow((void *)NVM_PROGRAM_PAGE, (void*)databuff);

    while (1)
        ;
    
    return 0;
}

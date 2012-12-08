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

enum addr{
    TRITIUM_ID          = 0x400,
    DRIVER_CONTROLS_ID  = 0x500
};

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
    
    NUCANInitializeModule(CAN1, FALSE);
    
    while (1){   
        BYTE goData[8];
        float motorVelocity = 1000.0; // m/s
        float motorCurrent  = 1.0;    // percentage
        PackTwoFloats(goData, motorVelocity, motorCurrent);
        NUCANTx(CAN1, DRIVER_CONTROLS_ID + 1, goData, 8);
        
        unsigned int id, len;
        BYTE rxData[8];
        if (!NUCANRx(CAN1, &id, rxData, &len)) {
            char buff[50];
            
            float float1, float2;
            switch (id) {
                case TRITIUM_ID + 2:
                    UnpackTwoFloats(rxData, &float1, &float2);
                    
                    sprintf(buff, "ID=%3d      V=%06.3f    I=%06.3f", id, float1, float2);
                    gotoXY(0,0);
                    LcdString(buff);
                    
                    break;
                default:
                    /*
                    sprintf(buff, "ID=%3d", id);
                    gotoXY(0,0);
                    LcdString(buff);
                    */
                    
                    break;
            }  // switch (id)
        }  // if (!NUCANRx)
    }  // while (1)
    
    return 0;
}

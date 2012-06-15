#include <plib.h>
#include "../include/NU32v2.h"
#include "../include/onewire.h"

/**********************************************************************
 * Function:        void init_OW (void)
 * PreCondition:    None
 * Input:     None 
 * Output:     None 
 * Overview:     Configure the OW_PIN as an open-drain output. 
 ***********************************************************************/
void init_OW(void)
{
    OW_PIN_ODC = 1;
}

/**********************************************************************
 * Function:        void drive_OW_low (void)
 * PreCondition:    None
 * Input:     None 
 * Output:     None 
 * Overview:     Configure the OW_PIN as Output and drive the OW_PIN LOW. 
 ***********************************************************************/
void drive_OW_low(void)       //Working
{
    OW_PIN_DIRECTION = 0;    
    OW_WRITE_PIN = 0;
}

 /**********************************************************************
 * Function:        void drive_OW_high (void)
 * PreCondition:    None
 * Input:     None 
 * Output:     None 
 * Overview:     Configure the OW_PIN as Output and drive the OW_PIN HIGH. 
 ***********************************************************************/
void drive_OW_high(void)     //Working
{
    OW_PIN_DIRECTION = 0;    
    OW_WRITE_PIN = 1; 
}

 /**********************************************************************
 * Function:        unsigned char read_OW (void)
 * PreCondition:    None
 * Input:     None 
 * Output:     Return the status of OW pin. 
 * Overview:     Configure as Input pin and Read the status of OW_PIN  
 ***********************************************************************/
unsigned char read_OW(void)
{
    unsigned char read_data=0;

    OW_PIN_DIRECTION = 1;
    if (OW_READ_PIN == 1)
    read_data = 1;
    else
    read_data = 0;

    return read_data;
}

/**********************************************************************
 * Function:        unsigned char OW_reset_pulse(void)
 * PreCondition:    None
 * Input:     None 
 * Output:     Return the Presense Pulse from the slave. 
 * Overview:     Initialization sequence start with reset pulse.
 *       This code generates reset sequence as per the protocol
 ***********************************************************************/
unsigned char OW_reset_pulse(void)  //Working
{
    unsigned char presence_detect;

    drive_OW_low();                 // Drive the bus low

    delay_us(480);                  // delay 480 microsecond (us)

    drive_OW_high();                // Release the bus

    delay_us(70);                   // delay 70 microsecond (us)

    presence_detect = read_OW();    //Sample for presence pulse from slave
    delay_us(410);                  // delay 410 microsecond (us)

    drive_OW_high ();               // Release the bus

    return presence_detect;
} 

 /**********************************************************************
 * Function:        void OW_write_bit (unsigned char write_data)
 * PreCondition:    None
 * Input:     Write a bit to 1-wire slave device.
 * Output:     None
 * Overview:     This function used to transmit a single bit to slave device.
 *       
 ***********************************************************************/
void OW_write_bit (unsigned char write_bit)
{
    if (write_bit) {
        drive_OW_low();     //writing a bit '1'
          
        delay_us(6);        // delay 6 microsecond (us)

        drive_OW_high();    // Release the bus
        delay_us(64);       // delay 64 microsecond (us)
        return;
    } else {
        drive_OW_low();     //writing a bit '0'  
                            // Drive the bus low
        delay_us(60);       // delay 60 microsecond (us)
            
        drive_OW_high();
                            // Release the bus
        delay_us(10);;      // delay 10 microsecond for recovery (us)
    }
} 
 
 /**********************************************************************
 * Function:        unsigned char OW_read_bit (void)
 * PreCondition:    None
 * Input:     None
 * Output:     Return the status of the OW PIN
 * Overview:     This function used to read a single bit from the slave device.
 *       
 ***********************************************************************/
unsigned char OW_read_bit (void)
{

    //reading a bit 
    drive_OW_low();         // Drive the bus low
    delay_us(6);            // delay 6 microsecond (us)
    drive_OW_high();          
    OW_PIN_DIRECTION = 1;   // Release the bus
    delay_us(9);
    if (PORTDbits.RD0 == 0) {
        delay_us(55);
        return 0;
    }
    if (PORTDbits.RD0 == 1) {
        delay_us(55);
        return 1;
    }

    return 0;
}

 /**********************************************************************
 * Function:        void OW_write_byte (unsigned char write_data)
 * PreCondition:    None
 * Input:     Send byte to 1-wire slave device
 * Output:     None
 * Overview:     This function used to transmit a complete byte to slave device.
 *       
 ***********************************************************************/
void OW_write_byte (unsigned char write_data)
{
    unsigned char loop;

    for (loop = 0; loop < 8; loop++) {
        OW_write_bit(write_data & 0x01);     //Sending LS-bit first
        write_data >>= 1;                    //right shift the data byte for the next bit to send
    } 
} 

 /**********************************************************************
 * Function:        unsigned char OW_read_byte (void)
 * PreCondition:    None
 * Input:     None
 * Output:     Return the read byte from slave device
 * Overview:     This function used to read a complete byte from the slave device.
 *       
 ***********************************************************************/
unsigned char OW_read_byte (void)
{
    unsigned char loop, result=0;

    for (loop = 0; loop < 8; loop++) {
        result >>= 1;     // shift the result to get it ready for the next bit to receive
        if (OW_read_bit())
            result |= 0x80;    // if result is one, then set MS-bit
    }
    return result;     
} 
 
 /**********************************************************************
 * Function:        unsigned char Detect_Slave_Device(void)
 * PreCondition:    None
 * Input:     None 
 * Output:     0 - Not Present   1 - Present  
 * Overview:       To check the presence of slave device.    
 ***********************************************************************/
unsigned char Detect_Slave_Device(void)
{
    if (!OW_reset_pulse())
        return 1;
    else   
        return 0;
} 

/**********************************************************************
 * Function:        unsigned char OW_crc(unsigned char x)
 * PreCondition:    None
 * Input:     Byte on which to perform CRC 
 * Output:     CRC Byte 
 * Overview:       To calculate CRC byte from    
 ***********************************************************************/
unsigned char OW_crc(unsigned char x)
{
    dowcrc = dscrc_table[dowcrc^x];
    return dowcrc;
}

/**********************************************************************
 * Function:        unsigned char Next(void)
 * PreCondition:    None
 * Input:     None
 * Output:     0 - No more devices    1 - More devices present, search is not yet complete
 * Overview:       Searches for next device on 1-Wire bus.    
 ***********************************************************************/
unsigned char Next(void)
{
    unsigned char m = 1;                //ROM bit index
    unsigned char n = 0;                //ROM byte index
    unsigned char k = 1;                //bit mask
    unsigned char x = 0;
    unsigned char discrepMarker = 0;    //discrepancy marker
    unsigned char g;                    //Output bit
    unsigned char nxt;                  //Return value
    int flag;
    
    nxt = 0;                            //Reset next flag to 0
    dowcrc = 0;                         //Reset the dowcrc
    flag = OW_reset_pulse();

    if (flag||doneFlag) {               //If no devices, return false
        lastDiscrep = 0;                //Reset the search
        return 0;
    }

    OW_write_byte(0xF0);                //Send SearchROM command
    do {
        x = 0;
        if (OW_read_bit() == 1)
            x = 2;
        delay_us(120);
        if (OW_read_bit() == 1)
            x |= 1;                    //AND its complement
            
        if (x == 3) {                  //There are no devices on the 1-Wire bus
            break;
        } else {
            if (x > 0) {               //All devices coupled have 0 or 1
                g = x >> 1;            //Bit write value for search
            } else {
                //If this discrepancy is before the last discrepancy on a previous
                //Next then pick the same as last time
                if (m < lastDiscrep)
                    g = ((RomBytes[n] & k) > 0);    
                else                                //If equal to last pick 1
                    g = (m == lastDiscrep);         //If not then pick 0
                //if 0 was picked then record position with mask k
                if (g == 0)
                    discrepMarker = m;
            }
            
            //Isolate bit in RomBytes[n] with mask k
            if (g == 1)
                RomBytes[n] |= k;
            else
                RomBytes[n] &= ~k;

            OW_write_bit(g);            //ROM search write

            m++;                        //Increment bit counter m
            k = k << 1;                 //and shift the bit mask k
            //If the mask is 0 then go to new RomBytes byte n and reset mask
            if (k == 0) {
                OW_crc(RomBytes[n]);    //Accumulate the CRC
                n++;
                k++;
            }
        }
    } while (n < 8);                    //Loop through until all RomBytes bytes 0-7

    if (m < (65 || dowcrc)) {           //If search was successful then
        lastDiscrep = 0;                //Reset the last discrepancy to zero
    } else {                            //Search was successful, so set lastDiscrep, lastOne, nxt;
        lastDiscrep = discrepMarker;
        doneFlag = (lastDiscrep == 0);
        nxt = 1;                        //Indicates search is not yet complete, more devices remain
    }

    return nxt;
}

/**********************************************************************
 * Function:        unsigned char First(void)
 * PreCondition:    None
 * Input:     None
 * Output:     Calls Next and returns its return value
 * Overview:       Resets current state of a ROM search and calls Next to find the first device on the bus    
 ***********************************************************************/
unsigned char First(void)
{
    lastDiscrep = 0; 
    doneFlag = 0; 
    return Next();    // Call Next and return it's return value; 
} 

/**********************************************************************
 * Function:        unsigned char FindDevices(void)
 * PreCondition:    None
 * Input:     None
 * Output:     Number of devices present
 * Overview:       Finds devices on bus and records their ROM serial numbers    
 ***********************************************************************/
unsigned char FindDevices(void)
{
    unsigned char m, cont = 0;
    if(!OW_reset_pulse()) { 
        if(First()) {           // Begins when at least one part found 
            numROMs = 0; 
            do { 
                numROMs++;  
                cont++; 
                for (m=0;m<8;m++)
                    FoundROM[numROMs][m] = RomBytes[m]; 
            } while (Next());   // Continues until no additional                              
        } 
    } 
    
    return cont; 
} 

/**********************************************************************
 * Function:        unsigned char Send_MatchROM(void)
 * PreCondition:    None
 * Input:     None
 * Output:     0 - No devices present    1 - Device(s) found
 * Overview:       Addresses device that matches the specified serial number found in numROMs
 ***********************************************************************/
unsigned char Send_MatchROM(void)
{
    unsigned char i;
    if (OW_reset_pulse())
        return 0;

    OW_write_byte(0x55);            //Match ROM

    for (i=0; i<8; i++)
        OW_write_byte(FoundROM[numROMs][i]);        //Send ROM code

    return 1;
}



 /********************************************************************************************
                   E N D     O F     1 W I R E . C  
 *********************************************************************************************/


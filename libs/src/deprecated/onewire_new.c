#include <GenericTypeDefs.h>
#include "../include/onewire_new.h"

/* CRC lookup table */
static const BYTE dscrcTab[] = {
   0,94,188,226,97,63,221,131,194,156,126,32,163,253,31,65, 
   157,195,33,127,252,162,64,30,95,1,227,189,62,96,130,220, 
   35,125,159,193,66,28,254,160,225,191,93,3,128,222,60,98, 
   190,224,2,92,223,129,99,61,124,34,192,158,29,67,161,255, 
   70,24,250,164,39,121,155,197,132,218,56,102,229,187,89,7, 
   219,133,103,57,186,228,6,88,25,71,165,251,120,38,196,154, 
   101,59,217,135,4,90,184,230,167,249,27,69,198,152,122,36, 
   248,166,68,26,153,199,37,123,58,100,134,216,91,5,231,185, 
   140,210,48,110,237,179,81,15,78,16,242,172,47,113,147,205, 
   17,79,173,243,112,46,204,146,211,141,111,49,178,236,14,80, 
   175,241,19,77,206,144,114,44,109,51,209,143,12,82,176,238, 
   50,108,142,208,83,13,239,177,240,174,76,18,145,207,45,115, 
   202,148,118,40,171,245,23,73,8,86,180,234,105,55,213,139, 
   87,9,235,181,54,104,138,212,149,203,41,119,244,170,72,22, 
   233,183,85,11,136,214,52,106,43,117,151,201,74,20,246,168, 
   116,42,200,150,21,75,169,247,182,232,10,84,215,137,107,53 
};

static BYTE dowcrc = 0x00;

static BYTE romNum[8];                  /* current ROM registration number discovered */
static unsigned int lastDiscrep;        /* bit index that identifies from which bit the (next) search discrepancy check should start */
static unsigned int lastFamilyDiscrep;  /* bit index identifying lastDiscrep within first 8-bit family code of ROM number */
static BOOL lastDeviceFlag;             /* indicates that previous search was last device */
static unsigned int numRoms = 0;

static inline void
driveOWLow(void) __attribute__((always_inline));

static inline void
driveOWHigh(void) __attribute__((always_inline));

static inline BIT
readOW(void) __attribute__((always_inline));

static void
owWriteBit(BIT bit);

static BIT
owReadBit(void);

static inline BYTE
owCrc(BYTE x) __attribute__((always_inline));

static inline void
crcReset(void) __attribute__((always_inline));

static inline void
resetSearchState(void) __attribute__((always_inline));

static inline BOOL
first(void) __attribute__((always_inline));

static inline BOOL
next(void) __attribute__((always_inline));

static BOOL
search(void);

static inline void
driveOWLow(void)
{
    OW_PIN_DIRECTION = 0;    
    OW_WRITE_PIN = 0;
}

static inline void
driveOWHigh(void)
{
    OW_PIN_DIRECTION = 0;    
    OW_WRITE_PIN = 1;
}

static inline BIT
readOW(void)
{
    OW_PIN_DIRECTION = 1;   /* Configure as input */
    return (BIT)OW_READ_PIN;
}

static void
owWriteBit(BIT bit)
{
    driveOWLow();
    if (bit)
        delay_us(6);
    else
        delay_us(60);
    driveOWHigh();
    if (bit)
        delay_us(64);
    else
        delay_us(10);
}

static BIT
owReadBit(void)
{
    driveOWLow();
    delay_us(6);
    driveOWHigh();
    OW_PIN_DIRECTION = 1;
    delay_us(9);
    if (!OW_READ_PIN) {
        delay_us(55);
        return 0;
    } else {
        delay_us(55);
        return 1;
    }
}

static inline BYTE
owCrc(BYTE x)
{
    dowcrc = dscrcTab[dowcrc^x];
    return dowcrc;
}

static inline void
crcReset(void)
{
    dowcrc = 0;
}

static inline void
resetSearchState(void)
{
    lastDiscrep = 0; 
    lastDeviceFlag = FALSE; 
    lastFamilyDiscrep = 0;
}

/* reset search and call search() to find first device on bus
 * 1 (TRUE):    device found, ROM number in romNum
 * 0 (FALSE):   no device present
 */
static inline BOOL
first(void)
{
    resetSearchState();
    return search();
}

static inline BOOL
next(void)
{
    /* don't change search state (lastDiscreps and lastDeviceFlag) */
    
    return search();
}

/* searches for all device IDs using binary search (which is equivalent to numerical order in this case)...
 * 0's come first...then backtrack and take '1' paths to find other ids
 * return of TRUE: device found, ROM number in romNum buffer
 * return of FALSE: device not found, end of search
 */
static BOOL
search(void)
{
    crcReset();
    BOOL searchResult = FALSE;
    
    if (lastDeviceFlag == FALSE) {
        if (!detectSlaveDevice()) {
            resetSearchState();
            return FALSE;
        }
        
        owWriteByte(OW_SEARCH_ROM);
        
        unsigned int romByteNum = 0;
        unsigned int lastZero = 0;  /* bit position of the last zero written where there was a discrepency */
        unsigned int idBitNum = 1;  /* the ROM bit number (1-64) currently being searched */
        BYTE romByteMask = 0x01;
        do {   /* iterate over bytes of ROM code */
            /* slaves start by sending bit 1 of ROM number followed by complement of bit 1 of ROM number */
            BIT idBit       = owReadBit();  /* AND of all of the id_bit_number bits of the devices still participating in the search */
            delay_us(120);
            BIT cmpIdBit    = owReadBit();  /* complement of idBit. AND of complement of all id_bit_number bits of participating devices */

            BIT searchDirection;    /* all devices with this bit stay in search, rest go into wait state for a 1-Wire reset */
            if (idBit && cmpIdBit) {    /* if no devices on bus... */
                break;
            } else {
                if (idBit != cmpIdBit) {    /* no discrepency, meaning all devices either have a 1 or 0 at this bit position in the id */
                    searchDirection = idBit;
                } else {
                    if (idBitNum < lastDiscrep) /* if discrepency is before last discrepancy on previous next(), then pick the same as last time */
                        searchDirection = ((romNum[romByteNum] & romByteMask) > 0);
                    else
                        searchDirection = (idBitNum == lastDiscrep);    /* take '1' path if same as last time, '0' path otherwise */
                    
                    if (searchDirection == 0) {
                        lastZero = idBitNum;    /* update position of last zero */
                        if (lastZero < 9)
                            lastFamilyDiscrep = lastZero;   /* "family" portion of id is first 8 bits */
                    }
                }
                
                if (searchDirection == 1)
                    romNum[romByteNum] |= romByteMask;
                else
                    romNum[romByteNum] &= (BYTE)(~romByteMask);
                
                owWriteBit(searchDirection);
                
                idBitNum++;
                romByteMask = (BYTE)(romByteMask<<1);
                
                /* if mask is 0 move to next byte and reset mask */
                if (romByteMask == 0x00) {
                    owCrc(romNum[romByteNum]);  /* accumalate CRC */
                    romByteNum++;
                    romByteMask = 0x01;
                }
            } /* if(idBit && cmpIdBit) {} else {       (devices on bus) */
        } while (romByteNum < 8);
        
        /* if search was successful... */
        if (!((idBitNum < 65) || (dowcrc != 0x00))) {
            lastDiscrep = lastZero;
            
            if (lastDiscrep == 0)
                lastDeviceFlag = TRUE;
            
            searchResult = TRUE;
        }
    }
    
    if (!searchResult || !romNum[0]) {
        resetSearchState();
        searchResult = FALSE;
    }
    
    return searchResult;
}

/* Configure the OW_PIN as an open-drain output */
inline void
initOW(void)
{
    OW_PIN_ODC = 1;
}

/* Sends reset pulse and looks for presence pulse from slave device(s) */
BIT
owResetPulse(void)
{
    BIT presence_detect;

    driveOWLow();
    delay_us(480);
    driveOWHigh();
    delay_us(70);
    presence_detect = readOW();
    delay_us(410);
    driveOWHigh();
    return presence_detect;
}

inline BOOL
detectSlaveDevice(void)
{
    return (!owResetPulse()) ? TRUE : FALSE;
}

void
owWriteByte (BYTE data)
{
    unsigned char i;
    for (i = 0; i < 8; i++) {
        owWriteBit(data & 0x01);
        data >>= 1;
    }    
}

inline void
owWriteBytes (BYTE *data, size_t len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        owWriteByte(data[i]);
}

BYTE
owReadByte (void)
{
    unsigned char i;
    BYTE result=0;
    for (i = 0; i < 8; i++) {
        result >>= 1;
        if (owReadBit())
            result |= 0x80;
    }
    return result;
}

inline void
owReadBytes (BYTE *data, size_t len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        data[i] = owReadByte();
}

/* returns number of devices on bus */
unsigned int
findDevices(void)
{
    unsigned int i = 0;
    if (detectSlaveDevice() == TRUE && first() == TRUE) {
        do {
            unsigned int j;
            for (j = 0; j < 8; j++)
                foundRoms[i][j] = romNum[j];
            i++;
        } while (next() == TRUE);
    }
    
    numRoms = i;
    return i;
}

BOOL
addressDevice(BYTE romCode[8])
{
    if (detectSlaveDevice() == FALSE)
        return FALSE;
    
    owWriteByte(OW_MATCH_ROM);
    
    unsigned int i;
    for (i = 0; i < 8; ++i)
        owWriteByte(romCode[i]);
    
    return TRUE;
}

int
getScratch(BYTE romCode[8], union scratch *data)
{
    if (addressDevice(romCode)==FALSE)
        return 1;
    
    owWriteByte(OW_RD_SCRATCH);
    
    crcReset();
    UINT8 i;
    for (i=0; i<8; i++)
    {
        data->byteArr[i] = owReadByte();
        owCrc(data->byteArr[i]);
    }
    data->byteArr[8] = owReadByte();
    
    if (data->crc != dowcrc)
        return 2;
        
    return 0;
}

void
startTempConversion(void)
{
    owResetPulse();
	owWriteByte(OW_SKIP_ROM);
	owWriteByte(OW_CONVERT_T);
}
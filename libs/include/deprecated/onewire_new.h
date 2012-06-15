#ifndef ONEWIRE_H_GUARD
#define ONEWIRE_H_GUARD

#include <GenericTypeDefs.h>
#include "NU32v2.h"
#include "Utility.h"

#if !(defined OW_PIN_LTR && defined OW_PIN_NUM)
    #define OW_PIN_LTR  D
    #define OW_PIN_NUM  0
#endif

/* PIN DEFINITIONS */
/* eg. D0 */
#define OW_PIN              PPCAT(OW_PIN_LTR, OW_PIN_NUM)
/* eg. TRISDbits.ODCD0 (Open-drain setting) */
#define OW_PIN_ODC          PPCAT(PPCAT(ODC, OW_PIN_LTR), PPCAT(bits.ODC, OW_PIN))
/* eg. TRISDbits.TRISD0 (Pin direction setting) */
#define OW_PIN_DIRECTION    PPCAT(PPCAT(TRIS, OW_PIN_LTR), PPCAT(bits.TRIS, OW_PIN))
/* eg. LATDbits.LATD0 (Instruction to write to pin) */
#define OW_WRITE_PIN        PPCAT(PPCAT(LAT, OW_PIN_LTR), PPCAT(bits.LAT, OW_PIN))
/* eg. PORTDbits.RD0 (Instruction to read from pin) */
#define OW_READ_PIN         PPCAT(PPCAT(PORT, OW_PIN_LTR), PPCAT(bits.R, OW_PIN))

/* ROM Commands */
#define OW_SEARCH_ROM   0xF0
#define OW_READ_ROM     0x33
#define OW_MATCH_ROM    0x55
#define OW_SKIP_ROM     0xCC
#define OW_ALARM_SRCH   0xEC

/* function commands */
#define OW_CONVERT_T    0x44
#define OW_WR_SCRATCH   0x4E    /* write scratchpad */
#define OW_RD_SCRATCH   0xBE
#define OW_CP_SCRATCH   0x48
#define OW_RECALL_EE    0xB8
#define OW_RD_PWR       0xB4

#define MAX_NUM_ROMS 64

/*Configuration Register*/
#define OW_RES_R0       (1<<6)
#define OW_RES_R1       (1<<7)
#define OW_RES_RESERVED (0x0F)
#define OW_RES_9BITS    (OW_RES_RESERVED)
#define OW_RES_10BITS   (OW_RES_RESERVED|OW_RES_R0)
#define OW_RES_11BITS   (OW_RES_RESERVED|OW_RES_R1)
#define OW_RES_12BITS   (OW_RES_RESERVED|OW_RES_R0|OW_RES_R1)

union scratch {
    struct {
        INT16 temp;
        BYTE user1;
        BYTE user2;
        BYTE config;
        unsigned reserved   :24;
        BYTE crc;
    };
    BYTE byteArr[9];
};

BYTE foundRoms[MAX_NUM_ROMS][8];

inline void
initOW(void) __attribute__((always_inline));

BIT
owResetPulse(void);

inline BOOL
detectSlaveDevice(void) __attribute__((always_inline));

void
owWriteByte (BYTE data);

inline void
owWriteBytes (BYTE *data, size_t len) __attribute__((always_inline));

BYTE
owReadByte (void);

inline void
owReadBytes (BYTE *data, size_t len) __attribute__((always_inline));

unsigned int
findDevices(void);

BOOL
addressDevice(BYTE romCode[8]);

int
getScratch(BYTE romCode[8], union scratch *data);

void
startTempConversion(void);

#endif

#ifndef __NU_DS18X20_H
#define __NU_DS18X20_H

#include <stdint.h>
#include "crc.h"
#include "onewire.h"
#include "timer.h"

extern const BYTE           DS18B20_FAMILY_CODE;
extern const BYTE           DS18S20_FAMILY_CODE;
extern const union romCode  ALL_DEVICES;
extern const union romCode  ALARM_DEVICES;

struct ds18b20ScratchPad {   /* excludes CRC */
    /* power-up temp value is 85C, 0b0000010101010000, or 0x0550 */
    /* this union corresponds to read-only memory */
    union {
        struct {
            BYTE rawTempLsb;    /* power-up state is 0x50 */
            BYTE rawTempMsb;    /* power-up state is 0x05 */
        };
        BYTE        rawTempBytes[2];
        int16_t     rawTemp;    /* divide by 2^4 (1<<4) to get actual temp */
    } rawTemp;

    /* this union corresponds to read/write memory */
    union {
        struct {
            int8_t  tempHigh;
            int8_t  tempLow;
        } alarm;
        /* "if the DS18B20 alarm function is not used, the TH and TL registers
         * can serve as general-purpose memory" */
        struct {
            BYTE    user1;
            BYTE    user2;
        };
    };
    
    /* read/write */
    union {
        struct {
            /* @TODO determine if this order needs to be flipped */
            unsigned    reserved    :1;
            unsigned    resolution  :2;
            unsigned    reserved2   :5;
        } __attribute__((packed));
        BYTE configByte;
    } config;

    /* read-only */
    BYTE reserved[3];
} __attribute__((packed));

STATIC_ASSERT(sizeof(struct ds18b20ScratchPad) == 8, SIZE_MISMATCH);

struct ds18s20ScratchPad {   /* excludes CRC */
    /* power-up temp value is 85C, 0b0000010101010000, or 0x0550 */
    /* this union corresponds to read-only memory */
    union {
        struct {
            BYTE rawTempLsb;    /* power-up state is 0x50 */
            BYTE rawTempMsb;    /* power-up state is 0x05 */
        };
        BYTE        rawTempBytes[2];
        int16_t     rawTemp;    /* divide by 2^4 (1<<4) to get actual temp */
    } rawTemp;

    /* this union corresponds to read/write memory */
    union {
        struct {
            int8_t  tempHigh;
            int8_t  tempLow;
        } alarm;
        /* "if the DS18B20 alarm function is not used, the TH and TL registers
         * can serve as general-purpose memory" */
        struct {
            BYTE    user1;
            BYTE    user2;
        };
    };

    BYTE reserved[2];

    /* read-only */
    BYTE countRemain;
    BYTE countPerDegreeC;
} __attribute__((packed));

STATIC_ASSERT(sizeof(struct ds18s20ScratchPad) == 8, SIZE_MISMATCH);

STATIC_ASSERT(sizeof(struct ds18b20ScratchPad) == sizeof(struct ds18s20ScratchPad),
        SIZE_MISMATCH);

union dsScratchPad {
    struct ds18b20ScratchPad ds18b20Scratch;
    struct ds18s20ScratchPad ds18s20Scratch;
};

enum dsResolution {
    /* 9-bit precision, 93.75ms max conversion time (tConv/8) */
    BITS_9              = 0,
    INC_0PT5C           = BITS_9,   /* increments of 0.5 degrees C */
    INC_HALFDEGREE      = BITS_9,   /* half a degree of precision */

    /* 10-bit precision, 187.5ms max conversion time (tConv/4) */
    BITS_10             = 1,
    INC_0PT25C          = BITS_10,
    INC_QUARTERDEGREE   = BITS_10,

    /* 11-bit precision, 375ms max conversion time (tConv/2) */
    BITS_11             = 2,
    INC_0PT125C         = BITS_11,
    INC_EIGHTHDEGREE    = BITS_11,

    /* 12-bit precision, 750ms max conversion time (tConv) */
    BITS_12             = 3,
    INC_0PT0625C        = BITS_12,
    INC_SIXTEENTHDEGREE = BITS_12,

    BITS_DEFAULT        = BITS_12,  /* value at power-up */
    INC_DEFAULT         = BITS_DEFAULT,
};

enum dsParasiticPower {
    PARASITIC_POWER_DISABLE = 0,
    PARASITIC_POWER_ENABLE
};

enum busyStatus {
    DEVICE_IDLE = 0,
    DEVICE_BUSY = 1,
};

struct ds18x20 {
    struct oneWire              ow;

    const struct vtbl_ds18b20   *op;

    enum dsParasiticPower       parasiticPower;
};

struct vtbl_ds18b20 {
    int32_t (*verify)               (struct ds18x20 *self, union romCode rc);
    int32_t (*pollBusyStatus)       (struct ds18x20 *self);
    int32_t (*readRom)              (const struct ds18x20 *self,
                                        union romCode *dst);
    int32_t (*findDevices)          (struct ds18x20 *self,
                                        union romCode *dst,
                                        size_t dstSizeBytes);
    int32_t (*findAlarmDevices)     (struct ds18x20 *self, union romCode *dst,
                                        size_t dstSizeBytes);
    int32_t (*startTempConversion)  (struct ds18x20 *self, union romCode rc);
    double  (*readTemp)             (struct ds18x20 *self, union romCode rc);
    int32_t (*writeScratch)         (struct ds18x20 *self, union dsScratchPad scratch,
                                        union romCode rc);
    int32_t (*readScratch)          (struct ds18x20 *self, union dsScratchPad *dst,
                                        union romCode rc);
    int32_t (*copyScratch)          (struct ds18x20 *self, union romCode rc);
    int32_t (*recall_e2)            (struct ds18x20 *self, union romCode rc);
    int32_t (*readPowerSupply)      (struct ds18x20 *self, union romCode rc);
    int32_t (*setLimits)            (struct ds18x20 *self, union romCode rc,
                                        float tLow, float tHigh);
    int32_t (*alarmSearch)          (struct ds18x20 *self);
};

int32_t
ds18x20_new(struct ds18x20 *self, IoPortId pinLtr, unsigned int pinNum,
            enum dsParasiticPower parasiticPower);

#endif

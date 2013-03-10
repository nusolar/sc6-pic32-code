#ifndef NU_DS18X20_H
#define NU_DS18X20_H

#include "compiler.h"
#include "nu_types.h"
#include "onewire.h"
#include "utility.h"

extern const u8             DS18B20_FAMILY_CODE;
extern const u8             DS18S20_FAMILY_CODE;
extern const union romcode  ALL_DEVICES;
extern const union romcode  ALARM_DEVICES;

PACKED struct ds18b20_scratch {   /* excludes CRC */
    /* power-up temp value is 85C, 0b0000010101010000, or 0x0550 */
    /* this union corresponds to read-only memory */
    union {
        struct {
            u8 lsb; /* power-up state is 0x50 */
            u8 msb; /* power-up state is 0x05 */
        };
        u8 bytes[2];
        s16 val;    /* divide by 2^4 (1<<4) to get actual temp */
    } raw_temp;

    /* this union corresponds to read/write memory */
    union {
        struct {
            s8 temp_high;
            s8 temp_low;
        } alarm;
        /* "if the DS18B20 alarm function is not used, the TH and TL registers
         * can serve as general-purpose memory" */
        struct {
            u8 user1;
            u8 user2;
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
        u8 byte;
    } config;

    /* read-only */
    u8 reserved[3];
};

STATIC_ASSERT(sizeof(struct ds18b20_scratch) == 8, SIZE_MISMATCH);

PACKED struct ds18s20_scratch {   /* excludes CRC */
    /* power-up temp value is 85C, 0b0000010101010000, or 0x0550 */
    /* this union corresponds to read-only memory */
    union {
        struct {
            u8 lsb; /* power-up state is 0x50 */
            u8 msb; /* power-up state is 0x05 */
        };
        u8 bytes[2];
        s16 val; /* divide by 2^4 (1<<4) to get actual temp */
    } raw_temp;

    /* this union corresponds to read/write memory */
    union {
        struct {
            s8 temp_high;
            s8 temp_low;
        } alarm;
        /* "if the DS18B20 alarm function is not used, the TH and TL registers
         * can serve as general-purpose memory" */
        struct {
            u8 user1;
            u8 user2;
        };
    };

    u8 reserved[2];

    /* read-only */
    u8 count_remain;
    u8 count_per_degree_c;
};

STATIC_ASSERT(sizeof(struct ds18s20_scratch) == 8, SIZE_MISMATCH);

STATIC_ASSERT(sizeof(struct ds18b20_scratch) == sizeof(struct ds18s20_scratch),
        SIZE_MISMATCH);

union ds_scratchpad {
    struct ds18b20_scratch ds18b20;
    struct ds18s20_scratch ds18s20;
};

enum ds_resolution {
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

enum ds_parasitic_power {
    PARASITIC_POWER_DISABLE = 0,
    PARASITIC_POWER_ENABLE
};

enum busy_status {
    DEVICE_IDLE = 0,
    DEVICE_BUSY = 1,
};

struct ds18x20 {
    struct w1 w1;
    enum ds_parasitic_power parasitic_power;
};

s32
ds_verify(struct ds18x20 *d, union romcode rc);

s32
ds_poll_busy_status(struct ds18x20 *d);

s32
ds_read_rom(const struct ds18x20 *d, union romcode *dst);

s32
ds_find_devices(struct ds18x20 *d, union romcode *dst, size_t n);

s32
ds_find_alarm_devices(struct ds18x20 *d, union romcode *dst, size_t n);

s32
ds_start_temp_conversion(struct ds18x20 *d, union romcode rc);

double
ds_read_temp(struct ds18x20 *d, union romcode rc);

s32
ds_write_scratch(struct ds18x20 *d, union ds_scratchpad scratch, union romcode rc);

s32
ds_read_scratch(struct ds18x20 *d, union ds_scratchpad *dst, union romcode rc);

s32
ds_copy_scratch(struct ds18x20 *d, union romcode rc);

s32
ds_recall_e2(struct ds18x20 *d, union romcode rc);

s32
ds_read_power_supply(struct ds18x20 *d, union romcode rc);

s32
ds_set_limits(struct ds18x20 *d, union romcode rc, float low, float high);

s32
ds_new(struct ds18x20 *d, struct pin pin, enum ds_parasitic_power parasitic_power);

#endif

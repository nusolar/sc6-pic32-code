#include "nu/component/ds18x20.h"
#include "nu/errorcodes.h"

#if 0

const u8            DS18B20_FAMILY_CODE = 0x28;
const u8            DS18S20_FAMILY_CODE = 0x10;
const union romcode ALL_DEVICES         = {0};
const union romcode ALARM_DEVICES       = {
    .bytes = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};

enum rom_cmds {
    DS_SEARCH_ROM       = 0xF0,
    DS_READ_ROM         = 0x33,
    DS_MATCH_ROM        = 0x55,
    DS_SKIP_ROM         = 0xCC,
    DS_ALARM_SEARCH     = 0xEC,
};

enum function_cmds {
    DS_CONVERT_T        = 0x44,
    DS_WRITE_SCRATCH    = 0x4E,
    DS_READ_SCRATCH     = 0xBE,
    DS_COPY_SCRATCH     = 0x48,
    DS_RECALL_E2        = 0xB8,
    DS_READ_POWER       = 0xB4,
};

static ALWAYSINLINE NONNULL() s32
init_sequence(const struct ds18x20 *d)
{
    return w1_reset(&(d->w1));
}

#define ds_init(d)  \
    init_sequence(d)

s32
ds_new(struct ds18x20 *d, struct nu__Pin pin,
       enum ds_parasitic_power parasitic_power)
{
    s32 err;

    if (!d)
        return -ENULPTR;

    d->parasitic_power = parasitic_power;
    if ((err = w1_new(&(d->w1), pin)) < 0)
        return err;

    return ds_init(d);
}

static ALWAYSINLINE NONNULL() s32
romcode_equals(const union romcode *rc1, const union romcode *rc2)
{
    u32 ui;
    for (ui = 0; ui < ARRAY_SIZE(rc1->bytes); ui++) {
        if (rc1->bytes[ui] != rc2->bytes[ui])
            return 0;
    }

    return 1;
}

s32
ds_verify(struct ds18x20 *d, union romcode rc)
{
    if (!d)
        return -ENULPTR;

    if (romcode_equals(&rc, &ALL_DEVICES) || romcode_equals(&rc, &ALARM_DEVICES))
        return -EINVAL;

    return w1_verify(&(d->w1), rc, DS_SEARCH_ROM);
}

s32
ds_poll_busy_status(struct ds18x20 *d)
{
    s32 rx_bit;
    if (!d)
        return -ENULPTR;
    if (d->parasitic_power == PARASITIC_POWER_ENABLE)
        return -EINVALIDOP;

    rx_bit = w1_rx_bit(&(d->w1));

    if (rx_bit == 0)
        return DEVICE_BUSY;
    else if (rx_bit == 1)
        return DEVICE_IDLE;
    else
        return rx_bit;
}

#define tx_cmd(d,cmd)   \
    (w1_tx_byte(&((d)->w1), cmd))

static ALWAYSINLINE NONNULL() s32
tx_rom_cmd(const struct ds18x20 *d, enum rom_cmds cmd)
{
    s32 err = 0;

    if ((err = init_sequence(d)) < 0)
        return err;
    if ((err = tx_cmd(d, cmd)) < 0)
        return err;

    return 0;
}

s32
ds_read_rom(const struct ds18x20 *d, union romcode *dst)
{
    s32 err = 0;
    u32 attempts = 3;

    if (!d || !dst)
        return -ENULPTR;

    memset(dst, 0, sizeof(*dst));

    while (attempts--) {
        if ((err = tx_rom_cmd(d, DS_READ_ROM)) < 0)
            return err;
        if ((err = w1_rx_check_crc(&(d->w1), dst, sizeof(*dst))) >= 0)
            break;
    }
    if (err < 0)
        return err;

    return 0;
}

s32
ds_find_devices(struct ds18x20 *d, union romcode *dst,
                size_t n)
{
    if (!d || (!dst && n != 0))
        return -ENULPTR;
    return w1_find_devices(&(d->w1), dst, n, DS_SEARCH_ROM);
}

s32
ds_find_alarm_devices(struct ds18x20 *d, union romcode *dst, size_t n)
{
    if (!d || (!dst && n != 0))
        return -ENULPTR;
    return w1_find_devices(&(d->w1), dst, n, DS_ALARM_SEARCH);
}

#define skip_rom(d) \
    tx_rom_cmd(d, DS_SKIP_ROM)

static ALWAYSINLINE NONNULL() s32
match_rom(const struct ds18x20 *d, union romcode *rc)
{
    s32 err = 0;

    if ((err = tx_rom_cmd(d, DS_MATCH_ROM)) < 0)
        return err;
    if ((err = w1_tx_with_crc(&(d->w1), rc, sizeof(*rc))) < 0)
        return err;

    return 0;
}

static ALWAYSINLINE NONNULL() s32
address_device(struct ds18x20 *d, union romcode *rc)
{
    if (romcode_equals(rc, &ALL_DEVICES))
        return skip_rom(d);
    else if (romcode_equals(rc, &ALARM_DEVICES))
        return ds_find_alarm_devices(d, NULL, 0);
    else
        return match_rom(d, rc);
}

s32
ds_start_temp_conversion(struct ds18x20 *d, union romcode rc)
{
    s32 err = 0;

    if (!d)
        return -ENULPTR;
    if ((err = address_device(d, &rc)) < 0)
        return err;
    if ((err = tx_cmd(d, DS_CONVERT_T)) < 0)
        return err;
    if (d->parasitic_power == PARASITIC_POWER_ENABLE)
        if ((err = w1_power_bus(&(d->w1))) < 0)
            return err;

    return 0;
}

s32
ds_read_scratch(struct ds18x20 *d, union ds_scratchpad *dst,
                union romcode rc)
{
    s32 err = 0;
    u32 maxAttempts = 3;

    if (!d || !dst)
        return -ENULPTR;
    if (romcode_equals(&rc, &ALL_DEVICES) || romcode_equals(&rc, &ALARM_DEVICES))
        return -EINVALIDOP;

    while (maxAttempts--) {
        if ((err = address_device(d, &rc)) < 0)
            return err;
        if ((err = tx_cmd(d, DS_READ_SCRATCH)) < 0)
            return err;
        if ((err = w1_rx_check_crc(&(d->w1), dst, sizeof(*dst))) >= 0)
            break;
    }

    return (err < 0) ? err : 0;
}

static ALWAYSINLINE double
rawTempToDegreesC(s16 raw_temp)
{
    return (double)(raw_temp/(1<<4));
}

static ALWAYSINLINE s16
degreesCToRawTemp(double temp_degrees_c)
{
    return (s16)(temp_degrees_c*(1<<4));
}

double
ds_read_temp(struct ds18x20 *d, union romcode rc)
{
    s32 err = 0;
    union ds_scratchpad scratch = {0};

    if (!d)
        return -ENULPTR;

    if ((err = ds_read_scratch(d, &scratch, rc)) < 0)
        return (double)err;

    if (rc.family == DS18B20_FAMILY_CODE) {
        return rawTempToDegreesC(scratch.ds18b20.raw_temp.val);
    } else if (rc.family == DS18S20_FAMILY_CODE) {
        BIT_CLEAR(scratch.ds18s20.raw_temp.val, 0);
        return (double)scratch.ds18s20.raw_temp.val/2-0.25+
                ((double)scratch.ds18s20.count_per_degree_c-(double)scratch.ds18s20.count_remain) /
                (double)scratch.ds18s20.count_per_degree_c;
    } else {
        return -EINVALIDOP;
    }
}

s32
ds_write_scratch(struct ds18x20 *d, union ds_scratchpad sp,
                    union romcode rc)
{
    s32 err = 0;

    if (!d)
        return -ENULPTR;
    if ((err = address_device(d, &rc)) < 0)
        return err;
    if ((err = tx_cmd(d, DS_WRITE_SCRATCH)) < 0)
        return err;
    if ((err = w1_tx_with_crc(&(d->w1), &sp, sizeof(sp))) < 0)
        return err;

    return 0;
}

/* scratchpad -> EEPROM */
s32
ds_copy_scratch(struct ds18x20 *d, union romcode rc)
{
    s32 err = 0;

    if (!d)
        return -ENULPTR;
    if ((err = address_device(d, &rc)) < 0)
        return err;
    if ((err = tx_cmd(d, DS_COPY_SCRATCH)) < 0)
        return err;
    if (d->parasitic_power == PARASITIC_POWER_ENABLE)
        if ((err = w1_power_bus(&(d->w1))) < 0)
            return err;

    return 0;
}

/* recall EEPROM
 * EEPROM -> scratchpad */
s32
ds_recall_e2(struct ds18x20 *d, union romcode rc)
{
    s32 err = 0;

    if (!d)
        return -ENULPTR;
    if ((err = address_device(d, &rc)) < 0)
        return err;
    if ((err = tx_cmd(d, DS_RECALL_E2)) < 0)
        return err;

    return 0;
}

s32
ds_read_power_supply(struct ds18x20 *d, union romcode rc)
{
    s32 err = 0, rx_bit;

    if (!d)
        return -ENULPTR;
    if ((err = address_device(d, &rc)) < 0)
        return err;
    if ((err = tx_cmd(d, DS_READ_POWER)) < 0)
        return err;
    rx_bit = w1_rx_bit(&(d->w1));

    if (rx_bit == 0)
        return PARASITIC_POWER_ENABLE;
    else if (rx_bit == 1)
        return PARASITIC_POWER_DISABLE;
    else
        return rx_bit;
}

#endif

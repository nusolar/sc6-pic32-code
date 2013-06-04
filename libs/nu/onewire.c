#include "nu/onewire.h"
#include "nu/crc.h"
#include "nu/errorcodes.h"
#include "nu/timer.h"
#include "nu/utility.h"

static const u32 owCrcTab[] = {
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

/*
 * CRC Info:
 * X^8 + X^5 + X^4 + 1
 * order: 8
 * polynom (hex): 31 (found on http://en.wikipedia.org/wiki/Cyclic_redundancy_check#Commonly_used_and_standardized_CRCs)
 * initial value (hex): 00
 * final XOR value (hex): 00
 * YES reverse data bytes
 * YES reverse CRC result before final XOR
 */
u32
w1_crc(const void *data, size_t n)
{
    if (!data)
        return 0xFFFFFFFF;

    return crcTableFast(owCrcTab, data, n, 8, CRC_DIRECT, 0x00, CRC_8_DALLAS,
            CRC_REVERSE_DATA_BYTES, CRC_REVERSE_BEFORE_FINAL_XOR, 0x00);
}

static ALWAYSINLINE NONNULL() void
drive_low(const struct w1 *w)
{
    PIN_CLEAR(w->pin);
    /* manipulate TRIS to pull to ground */
    PIN_SET_DIGITAL_OUT(w->pin);
}

static ALWAYSINLINE NONNULL() void
drive_high(const struct w1 *w)
{
    PIN_CLEAR(w->pin);
    /* manipulate TRIS to make it high impedance */
    PIN_SET_DIGITAL_IN(w->pin);
}

static s32
w1_init(const struct w1 *w)
{
    if (NULL == w)
        return -ENULPTR;

    /* Set LAT register, and then we'll manipulate TRIS register
     * via PORTSetPinsDigitalIn() and PORTSetPinsDigitalOut() to
     * control the bus, effectively toggling open-drain.
     */
    drive_low(w);

    return 0;
}

static ALWAYSINLINE s32
reset_search_state(struct w1 *w)
{
    if (NULL == w)
        return -ENULPTR;

    w->search_state.prev_search_was_last_dev    = false;
    w->search_state.last_discrep_bit            = 0;
    w->search_state.last_family_discrep_bit     = 0;

    return 0;
}

s32
w1_new(struct w1 *w, struct pin pin)
{
    s32 err = 0;

    if (NULL == w)
        return -ENULPTR;

    w->pin = pin;

    if ((err = reset_search_state(w)) < 0)
        return err;
    memset(w->search_state.romcode_crc.bytes, 0,
            sizeof(w->search_state.romcode_crc.bytes));

    return w1_init(w);
}

s32
w1_power_bus(const struct w1 *w)
{
    if (NULL == w)
        return -ENULPTR;

    PIN_SET(w->pin);
    PIN_SET_DIGITAL_OUT(w->pin);
    PIN_SET(w->pin); /* possibly not needed */

    return 0;
}

static ALWAYSINLINE NONNULL() void
tx_bit_nonnull(const struct w1 *w, bit b)
{
    drive_low(w);
    if (b) {
        delay_us(6);
        drive_high(w);
        delay_us(64);
    } else {
        delay_us(60);
        drive_high(w);
        delay_us(10);
    }
}

s32
w1_tx_bit(const struct w1 *w, bit b)
{
    if (NULL == w)
        return -ENULPTR;
    tx_bit_nonnull(w, b);
    return 0;
}

static ALWAYSINLINE NONNULL() void
tx_byte_nonnull(const struct w1 *w, u8 b)
{
    u32 ui;
    for (ui = 0; ui < 8; ++ui)
        tx_bit_nonnull(w, (b>>ui)&1);
}

s32
w1_tx_byte(const struct w1 *w, u8 b)
{
    if (!w)
        return -ENULPTR;
    tx_byte_nonnull(w, b);
    return 0;
}

s32
w1_tx(const struct w1 *w, const void *src, size_t n)
{
    const u8 *bytes = (const u8 *)src;
    u32 ui;

    if (!w || !src)
        return -ENULPTR;

    for (ui = 0; ui < n; ui++)
        tx_byte_nonnull(w, bytes[ui]);

    return 0;
}

s32
w1_tx_with_crc(const struct w1 *w, const void *src, size_t n)
{
    s32 err = 0;
    u8 crc;

    if ((err = w1_tx(w, src, n)) < 0)
        return err;

    crc = (u8)w1_crc(src, n);
    if ((err = w1_tx_byte(w, crc)) < 0)
        return err;

    return 0;
}

#define READ_BIT(w)    ((bit)PIN_READ((w)->pin))

static ALWAYSINLINE NONNULL() s32
rx_bit_nonnull(const struct w1 *w)
{
    bit result;
    drive_low(w);
    delay_us(6);
    drive_high(w);
    /*
     * This should theoretically be a 9us delay according to
     * http://www.maxim-ic.com/app-notes/index.mvp/id/126. However, using
     * 9us breaks everything. From trial and error, it seems to work using a
     * delay anywhere from 0us - 7us, so I've chosen to go with the middle
     * value out of these to avoid potential catastrophe. This should be
     * somewhere near the middle of the duration for which the ds18x20 is
     * asserting a bit.
     */
    delay_us(4);
    result = READ_BIT(w);
    delay_us(55);

    return result;
}

s32
w1_rx_bit(const struct w1 *w)
{
    if (NULL == w)
        return -ENULPTR;
    return rx_bit_nonnull(w);
}

s32
w1_rx(const struct w1 *w, void *dst, size_t n)
{
    u8 *dst_bytes = (u8 *)dst;
    u32 ui, uj;

    if (!w || !dst)
        return -ENULPTR;

    memset(dst_bytes, 0, n);

    for (ui = 0; ui < n; ui++)
        for (uj = 0; uj < 8; uj++)
            dst_bytes[ui] |= (u8)(rx_bit_nonnull(w)<<uj);

    return 0;
}

s32
w1_rx_check_crc(const struct w1 *w, void *dst, size_t len)
{
    s32 err = 0;
    u8 crc = 0x00;

    if ((err = w1_rx(w, dst, len)) < 0)
        return err;
    if ((err = w1_rx(w, &crc, sizeof(crc))) < 0)
        return err;
    if (crc != (u8)w1_crc(dst, len))
        return -ECRC;

    return 0;
}

s32
w1_reset(const struct w1 *w)
{
    bit devices_present;

    if (NULL == w)
        return -ENULPTR;

    drive_low(w);
    delay_us(480);
    drive_high(w);
    delay_us(70);
    devices_present = !READ_BIT(w);
    delay_us(410);
    /* driveHigh(self); */

    return devices_present ? 1 : -ENODEV;
}

static s32
presence_detected(const struct w1 *w)
{
    s32 err = 0;
    if ((err = w1_reset(w)) < 0)
        return err;

    return 1;
}

#define OW_NO_DEVS_LEFT_IN_SEARCH   12345

static s32
get_next_bit(struct w1 *w, u32 id_bit_number, u32 *last_zero_discrep_bit)
{
    bit id_bit, id_bit_complement, search_direction;

    if (NULL == w || NULL == last_zero_discrep_bit)
        return -ENULPTR;

    id_bit = w1_rx_bit(w); /* all devices send the bit */
    id_bit_complement = w1_rx_bit(w); /* ..then send the bit's complement */
    if ((1 == id_bit) && (1 == id_bit_complement)) { /* no devices left in search */
        reset_search_state(w);
        return OW_NO_DEVS_LEFT_IN_SEARCH;
    }

    if ((0 == id_bit) && (0 == id_bit_complement)) {
        if (id_bit_number == w->search_state.last_discrep_bit) {
            search_direction = 1;
        } else if (id_bit_number > w->search_state.last_discrep_bit) {
            search_direction = 0;
        } else {
            /* search direction becomes idBitNumber bit in romCodeAndCrc */
            search_direction =
                    w->search_state.romcode_crc.bytes[(id_bit_number-1)/8] &
                        (1<<((id_bit_number-1)%8));
        }

        if (0 == search_direction) {
            *last_zero_discrep_bit = id_bit_number;

            if (*last_zero_discrep_bit <= 8)
               w->search_state.last_family_discrep_bit = *last_zero_discrep_bit;
        }
    } else {
        search_direction = id_bit;
    }

    return search_direction;
}

static s32
search_rom(struct w1 *w, union romcode *dst, u8 search_rom_cmd)
{
    s32 err = 0;
    u8  crc;
    u32 id_bit_number           = 1;    /* 1 to 64 */
    /* bit posn of last zero written where there was a discrepancy
     * (1-64, unless there was no discrepancy in which case it remains 0 */
    u32 last_zero_discrep_bit   = 0; 

    if (NULL == w)
        return -ENULPTR;

    if ((err = presence_detected(w)) <= 0) { /* no presence detected or error */
        reset_search_state(w);
        return err;
    }
    
    if (TRUE == w->search_state.prev_search_was_last_dev) {
        reset_search_state(w);
        return 0;
    }

    if ((err = w1_tx_byte(w, search_rom_cmd)))
        return err;

    while (id_bit_number <= 64) {
        s32 next_bit = get_next_bit(w, id_bit_number,
                &last_zero_discrep_bit);
        bit nxtBit;
        
        if (OW_NO_DEVS_LEFT_IN_SEARCH == next_bit) {
            reset_search_state(w);
            return 0;
        }

        if (next_bit < 0)    /* error */
            return next_bit;

        if (next_bit)
            BIT_SET(w->search_state.romcode_crc.bytes[(id_bit_number-1)/8],
                    (id_bit_number-1)%8);
        else
            BIT_CLEAR(w->search_state.romcode_crc.bytes[(id_bit_number-1)/8],
                    (id_bit_number-1)%8);

        w1_tx_bit(w, (bit)next_bit);

        id_bit_number++;
    }

    w->search_state.last_discrep_bit = last_zero_discrep_bit;
    if (0 == w->search_state.last_discrep_bit)
        /* UNCHANGED - no discrepancies occured */
        w->search_state.prev_search_was_last_dev = TRUE;

    crc = w->search_state.romcode_crc.crc;
    if (NULL != dst)
        *dst = w->search_state.romcode_crc.rc;

    if (crc != (u8)w1_crc(&(w->search_state.romcode_crc.rc),
                                sizeof(w->search_state.romcode_crc.rc)))
        return -ECRC;

    return 1;
}

static s32
first(struct w1 *w, union romcode *dst, u8 search_rom_cmd)
{
    s32 err = 0;

    if (NULL == w)
        return -ENULPTR;
    if ((err = reset_search_state(w)) < 0)
        return err;
    
    return search_rom(w, dst, search_rom_cmd);
}

#define next(w,dst,searchrom)   \
    (search_rom((w), (dst), (searchrom)))

/**
 *
 * @param self
 * @param dst
 * @param dstSizeBytes
 * @param searchRomCmd
 * @return              Number of devices found or negative error code.
 */
s32
w1_find_devices(struct w1 *w, union romcode *dst, size_t n,
            u8 search_rom_cmd)
{
    s32 i, rc = 0;
    u32 array_size = n/sizeof(*dst);

    if (!w || (!dst && n != 0))
        return -ENULPTR;

    for (i = 0; i < INT_MAX; i++) {
        union romcode *rc_ptr = NULL;
        if (i < array_size)
            rc_ptr = &dst[i];

        if (i == 0) {
            if ((rc = first(w, rc_ptr, search_rom_cmd)) != 1)
                return rc;
        } else {
            rc = next(w, rc_ptr, search_rom_cmd);
            if (rc == 0)
                break;
            else if (rc < 0)    /* error */
                return rc;
        }
    }

    return i;
}

static s32
target_setup(struct w1 *w, u8 family)
{
    if (NULL == w)
        return -ENULPTR;

    w->search_state.last_discrep_bit            = 64;
    w->search_state.last_family_discrep_bit     = 0;
    w->search_state.prev_search_was_last_dev    = false;
    w->search_state.romcode_crc.rc.family       = family;
    memset(w->search_state.romcode_crc.rc.serial, 0,
            sizeof(w->search_state.romcode_crc.rc.serial));

    return 0;
}

s32
w1_find_family(struct w1 *w, union romcode *dst, size_t n,
                    u8 search_rom_cmd, u8 family)
{
    u32 ui, array_size = n/sizeof(*dst);

    if (NULL == w || (NULL == dst && n != 0))
        return -ENULPTR;

    target_setup(w, family);

    for (ui = 0; ui < INT_MAX; ui++) {
        union romcode rc;
        s32 next = next(w, &rc, search_rom_cmd);
        if (0 == next || rc.family != family)
            break;
        else if (next < 0)    /* error */
            return next;
        else if (ui < array_size)
            dst[ui] = rc;
    }

    return (s32)ui;
}

static s32
family_skip_setup(struct w1 *w)
{
    if (NULL == w)
        return -ENULPTR;

    w->search_state.last_discrep_bit = w->search_state.last_family_discrep_bit;
    w->search_state.last_family_discrep_bit = 0;
    w->search_state.prev_search_was_last_dev = false;

    return 0;
}

s32
w1_find_skip_family(struct w1 *w, union romcode *dst, size_t n,
                    u8 search_rom_cmd, u8 family)
{
    union romcode dummy_rc;
    u32 ui, array_size = n/sizeof(*dst);

    if (NULL == w || (NULL == dst && n != 0))
        return -ENULPTR;

    reset_search_state(w);
    target_setup(w, family);

    memset(&dummy_rc, 0, sizeof(dummy_rc));
    next(w, &dummy_rc, search_rom_cmd);

    if (dummy_rc.family == family) {
        family_skip_setup(w);
    } else {
        reset_search_state(w);
    }

    for (ui = 0; ui < INT_MAX; ui++) {
        union romcode current_rc;
        int32_t nxtResult = next(w, &current_rc, search_rom_cmd);
        if (nxtResult == 0 || current_rc.family == family)
            break;
        else if (nxtResult < 0)    /* error */
            return nxtResult;
        else if (ui < array_size)
            dst[ui] = current_rc;
    }

    return (s32)ui;
}

/**
 * @return              1 if specified romCode 'rc' is found, 0 or error code
 *                          otherwise.
 */
s32
w1_verify(struct w1 *w, union romcode rc, u8 search_rom_cmd)
{
    s32 err = 0;
    union romcode searhrom_result_rc;

    if (NULL == w)
        return -ENULPTR;

    w->search_state.romcode_crc.rc              = rc;
    w->search_state.romcode_crc.crc             = w1_crc(&rc, sizeof(rc));

    w->search_state.last_discrep_bit            = 64;
    w->search_state.last_family_discrep_bit     = 0;
    w->search_state.prev_search_was_last_dev    = false;

    if ((err = search_rom(w, &searhrom_result_rc, search_rom_cmd) <= 0))
        return err;
    if (!memcmp(&rc, &searhrom_result_rc, sizeof(rc)))
        return 1;

    return 0;
}

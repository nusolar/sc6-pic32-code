#ifndef NU_ONEWIRE_H
#define NU_ONEWIRE_H 1

#include "nu/types.h"
#include "nu/pinctl.h"

union romcode {
    struct {
        u8 family;
        u8 serial[6];
    };
    u8 bytes[7];
};

struct w1 {
    struct {
        u32     last_discrep_bit;
        u32     last_family_discrep_bit;
        bool    prev_search_was_last_dev;
        union {
            u8 bytes[8];
            struct {
                union romcode   rc;
                u8              crc;
            };
        } romcode_crc;
    } search_state;
    struct nu_pin pin;
};

#define SEARCH_STATE_INIT(last_discrep_bit, last_family_discrep_bit, prev_search_was_last_dev, romcode_bytes)   \
    {.last_discrep_bit = last_discrep_bit, .last_family_discrep_bit = last_family_discrep_bit, \
    .prev_search_was_last_dev = prev_search_was_last_dev, .romcode_crc.bytes = romcode_bytes}

#define W1_INIT(pin_ltr, pin_num)   \
    { .pin = PIN_INIT(pin_ltr, pin_num) }

s32
w1_power_bus(const struct w1 *w);

s32
w1_tx_bit(const struct w1 *w, bit b);

s32
w1_tx_byte(const struct w1 *w, u8 b);

s32
w1_tx(const struct w1 *w, const void *src, size_t n);

s32
w1_tx_with_crc(const struct w1 *w, const void *src, size_t n);

s32
w1_rx_bit(const struct w1 *w);

s32
w1_rx(const struct w1 *w, void *dst, size_t n);

s32
w1_rx_check_crc(const struct w1 *w, void *dst, size_t n);

s32
w1_reset(const struct w1 *w);

s32
w1_find_devices(struct w1 *w, union romcode *dst, size_t n,
                byte search_rom_cmd);

s32
w1_find_family(struct w1 *w, union romcode *dst, size_t n,
               byte search_rom_cmd, byte family);

s32
w1_find_skip_family(struct w1 *w, union romcode *dst, size_t n_code,
                    byte search_rom_cmd, byte family);

s32
w1_verify(struct w1 *self, union romcode rc, byte search_rom_cmd);

u32
w1_crc(const void *data, size_t n);

s32
w1_new(struct w1 *w, struct nu_pin pin);

#endif

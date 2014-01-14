#ifndef NU_AD7685_H
#define NU_AD7685_H 1

#include "nu/peripheral/spi.h"
#include "nu/peripheral/pinctl.h"
#include "nu/compiler.h"
#include "nu/types.h"

/**
 *  The Analog-to-Digital Converter used on the BMS current sensor
 */
enum nu_ad7685_options {
    NU_AD7685_NONE = 0,
    /* wire config */
    NU_AD7685_THREE_WIRE = 0,
    NU_AD7685_FOUR_WIRE  = 1<<0,
    NU_AD7685_CHAIN_MODE = 1<<1 | 1<<0,

    /* busy indicator */
    NU_AD7685_NO_BUSY_INDICATOR = 0,
    NU_AD7685_BUSY_INDICATOR    = 1<<2
};

struct nu_ad7685 {
    struct nu_spi spi;
    struct nu_pin convert_pin;
    u32 num_devices;
    enum nu_ad7685_options opt;
};
/* refer to the AD7685 documentation to determine which modes require the use
 * of chip-select */
#define NU_AD7685_CS_INIT(chn, cs_pin, convert_pin, num_devices, opt) \
    { \
    NU_SPI_CS_INIT(cs_pin, chn, NU_SPI_DEFAULT), \
    convert_pin, \
    (num_devices), \
    (opt) \
    }
#define NU_AD7685_INIT(chn, convert_pin, _num_devices, opt) \
    NU_AD7685_CS_INIT(chn, NU_PIN_INIT(NU_PIN_DEFAULT), convert_pin, _num_devices, opt)

#define NU_AD7685_CS(name, chn, cs_pin, convert_pin, num_devices, opt)   \
    struct nu_ad7685 name = NU_AD7685_CS_INIT(chn, cs_pin, convert_pin, num_devices, opt)
#define NU_AD7685(name, chn, convert_pin, num_devices, opt)   \
    struct nu_ad7685 name = NU_AD7685_INIT(chn, convert_pin, num_devices, opt)

s32 MUST_CHECK
nu_ad7685_setup(struct nu_ad7685 *self);

/* microvolts */
void
nu_ad7685_convert_read_uv(const struct nu_ad7685 *a, u32 *dst);

#endif

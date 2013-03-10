#ifndef NU_AD7685_H
#define NU_AD7685_H 1

#include "compiler.h"
#include "nu_types.h"
#include "pinctl.h"
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#define NU_AD7685_CS_INIT(chn, cs_ltr, cs_num, convert_ltr, convert_num, _num_devices, opt) \
    { \
    NU_SPI_CS_INIT(cs_ltr, cs_num, chn, NU_SPI_DEFAULT), \
    NU_PIN_INIT(convert_ltr, convert_num), \
    (_num_devices), \
    (opt) \
    }
#define NU_AD7685_INIT(chn, convert_ltr, convert_num, _num_devices, opt) \
    NU_AD7685_CS_INIT(chn, IOPORT_A, BIT_0, convert_ltr, convert_num, _num_devices, opt)

#define NU_AD7685_CS(name, chn, cs_ltr, cs_num, convert_ltr, convert_num, num_devices, opt)   \
    struct nu_ad7685 name = NU_AD7685_CS_INIT(chn, cs_ltr, cs_num, convert_ltr, convert_num, num_devices, opt)
#define NU_AD7685(name, chn, convert_ltr, convert_num, num_devices, opt)   \
    struct nu_ad7685 name = NU_AD7685_INIT(chn, convert_ltr, convert_num, num_devices, opt)

static INLINE void
NU_INIT_AD7685_CS(struct nu_ad7685 *a, SpiChannel chn, IoPortId cs_ltr, u32 cs_num,
        IoPortId convert_ltr, u32 convert_num, u32 num_devices,
        enum nu_ad7685_options opt)
{
    NU_INIT_SPI_CS(&(a->spi), cs_ltr, cs_num, chn, NU_SPI_DEFAULT);
    NU_INIT_PIN(&(a->convert_pin), convert_ltr, convert_num);
    a->num_devices = num_devices;
    a->opt = opt;
}

static INLINE void
NU_INIT_AD7685(struct nu_ad7685 *a, SpiChannel chn,
        IoPortId convert_ltr, u32 convert_num, u32 num_devices,
        enum nu_ad7685_options opt)
{
    NU_INIT_AD7685_CS(a, chn, IOPORT_A, BIT_0, convert_ltr, convert_num,
            num_devices, opt);
}

s32 MUST_CHECK
nu_ad7685_setup(const struct nu_ad7685 *a);

/* microvolts */
void
nu_ad7685_convert_read_uv(const struct nu_ad7685 *a, u32 *dst);

#ifdef __cplusplus
}   /* extern c */
#endif

#endif

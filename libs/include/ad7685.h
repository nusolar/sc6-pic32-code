#ifndef NU_AD7685_H
#define NU_AD7685_H

#include "compiler.h"
#include "nu_types.h"
#include "pinctl.h"
#include "spi.h"

enum wire_config {
    THREE_WIRE,
    FOUR_WIRE,
    CHAIN_MODE
};

enum busy_indic {
    NO_BUSY_INDICATOR,
    USE_BUSY_INDICATOR
};

struct ad7685 {
    struct spi spi;
    struct pin convert_pin;
    u32 num_devices;
    enum wire_config wire_config;
    enum busy_indic busy_indic;
};

/* refer to the AD7685 documentation to determine which modes require the use
 * of chip-select */

#define AD7685_CS_INIT(chn, cs_ltr, cs_num, convert_ltr, convert_num, _num_devices, _wire_config, _busy_indic)  \
    {                                                           \
    .spi = SPI_CS_INIT(cs_ltr, cs_num, chn, AUTO_CS_DISABLE),   \
    .convert_pin = PIN_INIT(convert_ltr, convert_num),          \
    .num_devices = (_num_devices),                              \
    .wire_config = (_wire_config),                              \
    .busy_indic  = (_busy_indic)                                \
    }
#define AD7685_INIT(chn, convert_ltr, convert_num, _num_devices, _wire_config, _busy_indic)   \
    AD7685_CS_INIT(chn, IOPORT_A, BIT_0, convert_ltr, convert_num, _num_devices, _wire_config, _busy_indic)

#define AD7685_CS(name, chn, cs_ltr, cs_num, convert_ltr, convert_num, num_devices, wire_config, busy_indic)   \
    struct ad7685 name = AD7685_CS_INIT(chn, cs_ltr, cs_num, convert_ltr, convert_num, num_devices, wire_config, busy_indic)
#define AD7685(name, chn, convert_ltr, convert_num, num_devices, wire_config, busy_indic)   \
    struct ad7685 name = AD7685_INIT(chn, convert_ltr, convert_num, num_devices, wire_config, busy_indic)

static INLINE void
INIT_AD7685_CS(struct ad7685 *a, SpiChannel chn, IoPortId cs_ltr, u32 cs_num,
        IoPortId convert_ltr, u32 convert_num, u32 num_devices,
        enum wire_config wire_config, enum busy_indic busy_indic)
{
    INIT_SPI_CS(&(a->spi), cs_ltr, cs_num, chn, AUTO_CS_DISABLE);
    INIT_PIN(&(a->convert_pin), convert_ltr, convert_num);
    a->num_devices = num_devices;
    a->wire_config = wire_config;
    a->busy_indic = busy_indic;
}

static INLINE void
INIT_AD7685(struct ad7685 *a, SpiChannel chn,
        IoPortId convert_ltr, u32 convert_num, u32 num_devices,
        enum wire_config wire_config, enum busy_indic busy_indic)
{
    INIT_AD7685_CS(a, chn, IOPORT_A, BIT_0, convert_ltr, convert_num,
            num_devices, wire_config, busy_indic);
}

s32 MUST_CHECK
ad7685_setup(const struct ad7685 *a);

/* microvolts */
void
ad7685_convert_read_uv(const struct ad7685 *a, u32 *dst)

#endif

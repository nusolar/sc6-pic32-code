#include "nu/component/mcp49x2.h"
#include "nu/peripheral/spi.h"
#include "nu/errorcodes.h"
#include "nu/utility.h"

    #pragma pack(push, 1)
struct PACKED nu__Mcp49x2__write_cmd {
    u32 data                :12;
    /* Output Shutdown Control bit; 1 = active */
    u32 shdn                :1;
    /* Output Gain Selection bit */
    u32 output_gain_select  :1;
    /* V_REF Input Buffer Control bit */
    u32 vref_buf_enable     :1;
    /* DAC_A or DAC_B Selection bit */
    u32 dac_select          :1;
};
    #pragma pack(pop)

STATIC_ASSERT(2 == sizeof(struct nu__Mcp49x2__write_cmd), SIZE_MISMATCH);

void
nu__Mcp49x2__write(const struct nu__Mcp49x2 *m, enum nu__Mcp49x2__dac dac,
        bool vref_buf_enable, enum nu__Mcp49x2__output_gain gain,
        UNUSED bool shutdown, u16 data)
{
    struct nu__Mcp49x2__write_cmd cmd;
    u16 tmp;

    cmd.data = BITFIELD_CAST(data, 12);
    cmd.shdn = !shutdown;
    cmd.output_gain_select = gain;
    cmd.vref_buf_enable = vref_buf_enable;
    cmd.dac_select = dac;

    memcpy(&tmp, &cmd, sizeof(tmp));

    nu__Spi__cs_low(&(m->spi));
    SpiChnPutC(m->spi.platform.chn, tmp);
    while (!SpiChnTxBuffEmpty(m->spi.platform.chn))
        Nop();
    nu__Spi__cs_high(&(m->spi));
}

MUST_CHECK s32
nu__Mcp49x2__setup(struct nu__Mcp49x2 *m, u32 bitrate)
{
    if (unlikely(bitrate > 20000000))
        return -EINVAL;
	struct nu__Spi__PlatformSetupArgs args = {(SpiOpenFlags)(SPI_OPEN_MSTEN | SPI_OPEN_CKE_REV | SPI_OPEN_MODE16 | SPI_OPEN_ON)};
    nu__Spi__setup(&(m->spi), bitrate, &args);

    return 0;
}

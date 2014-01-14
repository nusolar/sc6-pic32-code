#ifndef NU_MCP49X2_H
#define NU_MCP49X2_H 1

#include "nu/peripheral/spi.h"
#include "nu/peripheral/pinctl.h"
#include "nu/types.h"
#include "nu/compiler.h"

enum nu__Mcp49x2__dac {
    NU_MCP49_DAC_A = 0,
    NU_MCP49_DAC_B
};

enum nu__Mcp49x2__output_gain {
    NU_MCP49_OUTPUT_2X = 0,
    NU_MCP49_OUTPUT_1X
};

struct nu__Mcp49x2 {
    struct nu__Pin ldac;
    struct nu__Pin shdn;
    struct nu__Spi spi;
};
#define NU_MCP49X2_INIT(ldac_ltr, ldac_num, shdn_ltr, shdn_num, cs_ltr, cs_num, chn) \
    { \
    NU_PIN_INIT(ldac_ltr, ldac_num), \
    NU_PIN_INIT(shdn_ltr, shdn_num), \
    NU_SPI_CS_INIT(cs_ltr, cs_num, chn, NU_SPI_DEFAULT) \
    }
#define NU_MCP49X2(name, ldac_ltr, ldac_num, shdn_ltr, shdn_num, cs_ltr, cs_num, chn)  \
    struct nu__Mcp49x2 name = NU_MCP49X2_INIT(ldac_ltr, ldac_num, shdn_ltr, shdn_num, cs_ltr, cs_num, chn)

/* max bitrate 20MHz */
MUST_CHECK s32
nu__Mcp49x2__setup(struct nu__Mcp49x2 *m, u32 bitrate);

void
nu__Mcp49x2__write(const struct nu__Mcp49x2 *m, enum nu__Mcp49x2__dac dac, bool vref_buf_enable, enum nu__Mcp49x2__output_gain gain, bool shutdown, u16 data);

#endif

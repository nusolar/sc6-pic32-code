#ifndef NU_MCP49X2_H
#define NU_MCP49X2_H 1

#include "compiler.h"
#include "nu_types.h"
#include "pinctl.h"
#include "spi.h"

enum nu_mcp49_dac {
    NU_MCP49_DAC_A = 0,
    NU_MCP49_DAC_B
};

enum nu_mcp49_output_gain {
    NU_MCP49_OUTPUT_2X = 0,
    NU_MCP49_OUTPUT_1X
};

struct nu_mcp49x2 {
    struct nu_pin ldac;
    struct nu_pin shdn;
    struct nu_spi spi;
};
#define NU_MCP49X2_INIT(ldac_ltr, ldac_num, shdn_ltr, shdn_num, cs_ltr, cs_num, chn) \
    { \
    NU_PIN_INIT(ldac_ltr, ldac_num), \
    NU_PIN_INIT(shdn_ltr, shdn_num), \
    NU_SPI_CS_INIT(cs_ltr, cs_num, chn, NU_SPI_DEFAULT) \
    }
#define NU_MCP49X2(name, ldac_ltr, ldac_num, shdn_ltr, shdn_num, cs_ltr, cs_num, chn)  \
    struct nu_mcp49x2 name = NU_MCP49X2_INIT(ldac_ltr, ldac_num, shdn_ltr, shdn_num, cs_ltr, cs_num, chn)

/* max bitrate 20MHz */
MUST_CHECK s32
nu_mcp49x2_setup(const struct nu_mcp49x2 *m, u32 bitrate);

void
nu_mcp49x2_write(const struct nu_mcp49x2 *m, enum nu_mcp49_dac dac, bool vref_buf_enable, enum nu_mcp49_output_gain gain, bool shutdown, u16 data);

#endif
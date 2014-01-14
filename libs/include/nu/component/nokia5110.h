#ifndef NU_NOKIA5110_H
#define NU_NOKIA5110_H 1

#include "nu/peripheral/spi.h"
#include "nu/peripheral/pinctl.h"
#include "nu/utility.h"
#include "nu/error_reporting.h"
#include "nu/types.h"
#include "nu/compiler.h"

/***********************************
*  Schematic:
*  LCD 1-Vcc		----->	NU32v2 3.3V
*  LCD 2-GND		----->	NU32v2 GND
*  LCD 3-SCE/CS     ----->	NU32v2 E2
*  LCD 4-RST		----->	NU32v2 E1
*  LCD 5-D/C		----->	NU32v2 E0
*  LCD 6-DNK(MOSI)/SDO	----->	NU32v2 F8
*  LCD 7-SCLK		----->	NU32v2 D15
*  LCD 8-LED		----->	330 ohm	----->	5V
*  (8-LED for backlight only if desired)
************************************/

/* not in any particular order... */
enum nu__Nokia__CmdFuncSetOptions {
    NU_NOKIA_DC_DATA                = 0,
    NU_NOKIA_DC_COMMAND             = 1<<0,

    NU_NOKIA_ADDRESSING_HORIZONTAL  = 0,
    NU_NOKIA_ADDRESSING_VERTICAL    = 1<<1,

    NU_NOKIA_ACTIVE                 = 0,
    NU_NOKIA_POWERDOWN              = 1<<2,

    NU_NOKIA_DISP_MODE_BLANK        = 0,
    NU_NOKIA_DISP_MODE_NORMAL       = 1<<3,
    NU_NOKIA_DISP_MODE_ALL_ON       = 1<<4,
    NU_NOKIA_DISP_MODE_INVERSE      = 1<<4 | 1<<3,

    NU_NOKIA_INSTRUCTIONS_BASIC     = 0,
    NU_NOKIA_INSTRUCTIONS_EXTENDED  = 1<<5,

    NU_NOKIA_TEMP_COEFF_0           = 0,
    NU_NOKIA_TEMP_COEFF_1           = 1<<6,
    NU_NOKIA_TEMP_COEFF_2           = 1<<7,
    NU_NOKIA_TEMP_COEFF_3           = 1<<7 | 1<<6
};

struct nu__Nokia5110 {
    struct nu__ErrorReportingDev erd;
    struct nu__Spi spi;
    struct nu__Pin pin_reset;   /* active LOW*/
    struct nu__Pin pin_dc;      /* data/command input */
};
#define _nu_erd_to_nokia5110(erdp) \
    container_of((erdp), struct nu__Nokia5110, erd)
#define NU_NOKIA_ERD_INIT(min_priority, chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num) \
    { \
    NU_ERD_INIT(min_priority, &nokia_erd_ops), \
    NU_SPI_CS_INIT(cs_ltr, cs_num, chn), \
    NU_PIN_INIT(reset_ltr, reset_num), \
    NU_PIN_INIT(dc_ltr, dc_num) \
    }
#define NU_NOKIA_INIT(chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num)   \
    NU_NOKIA_ERD_INIT(REP_DEBUG, chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num)
#define NU_NOKIA_ERD(name, min_priority, chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num)  \
    struct nu__Nokia5110 name = NU_NOKIA_ERD_INIT(min_priority, chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num)
#define NU_NOKIA(name, chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num)  \
    struct nu__Nokia5110 name = NU_NOKIA_INIT(chn, cs_ltr, cs_num, reset_ltr, reset_num, dc_ltr, dc_num)

void
nu__Nokia__setup(struct nu__Nokia5110 *n);

void
nu__Nokia__cmd_func_set(const struct nu__Nokia5110 *self,
                    enum nu__Nokia__CmdFuncSetOptions opt);

void
nu__Nokia__cmd_set_vop(const struct nu__Nokia5110 *self, u8 vop);

/* ALIAS("nu__Nokia__cmd_set_vop") void*/
#define nu__Nokia__cmd_set_contrast(self, vop) nu__Nokia__cmd_set_vop(self, vop)

void
nu__Nokia__cmd_set_temp_coeff(const struct nu__Nokia5110 *n, enum nu__Nokia__CmdFuncSetOptions coeff);

void
nu__Nokia__cmd_set_bias(const struct nu__Nokia5110 *n, u8 bias);

void
nu__Nokia__cmd_set_disp_mode(const struct nu__Nokia5110 *n, enum nu__Nokia__CmdFuncSetOptions mode);

void
nu__Nokia__putc(const struct nu__Nokia5110 *n, char c);

void PRINTF(2,3)
nu__Nokia__printf(const struct nu__Nokia5110 *n, const char *fmt, ...);

void
nu__Nokia__puts(const struct nu__Nokia5110 *n, const char *s);

void
nu__Nokia__goto_xy(const struct nu__Nokia5110 *n, u8 x, u8 y);

void
nu__Nokia__set_pixel(const struct nu__Nokia5110 *n, u8 x, u8 y);

void
nu__Nokia__clear(const struct nu__Nokia5110 *n);

#endif

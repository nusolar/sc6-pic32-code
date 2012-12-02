#ifndef NU_NOKIA5110_H
#define NU_NOKIA5110_H

#include "compiler.h"
#include "error_reporting.h"
#include "pinctl.h"
#include "spi.h"
#include "utility.h"

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

enum nokia_dc {
    DATA,
    COMMAND
};

enum nokia_addr_mode {
    HORIZONTAL_ADDRESSING = 0,
    VERTICAL_ADDRESSING = 1
};

enum nokia_chip_active {
    ACTIVE = 0,
    POWERDOWN = 1
};

enum nokia_display_mode {
    BLANK   = 0x00,
    NORMAL  = 0x04,
    ALL_ON  = 0x01,
    INVERSE = 0x05
};

enum nokia_extended {
    BASIC_INSTRUCTIONS = 0,
    EXTENDED_INSTRUCTIONS = 1
};

enum nokia_temp_coeff {
    TEMP_COEFF_0 = 0,
    TEMP_COEFF_1,
    TEMP_COEFF_2,
    TEMP_COEFF_3
};

struct nokia5110 {
    struct error_reporting_dev erd;
    struct spiPort spi;
    struct pin pin_reset;   /* active LOW*/
    struct pin pin_dc;      /* data/command input */
};

#define error_reporting_dev_to_nokia5110(erdp)   \
        container_of((erdp), struct nokia5110, erd)

int32_t
nokia5110_new(struct nokia5110 *self, SpiChannel _chn,
              struct pin pin_cs, struct pin pin_reset,
              struct pin pin_dc);

void
nokia_cmd_func_set(const struct nokia5110 *self,
                    enum nokia_addr_mode addr_mode,
                    enum nokia_chip_active active,
                    enum nokia_extended use_extended);

void
nokia_cmd_set_vop(const struct nokia5110 *self, uint8_t vop);

/* alias for nokia_cmd_set_vop */
#define nokia_cmd_set_contrast(n, c)    \
    nokia_cmd_set_vop(n, c)

void
nokia_cmd_set_temp_coeff(const struct nokia5110 *self,
                         enum nokia_temp_coeff coeff);

void
nokia_cmd_set_bias(const struct nokia5110 *self, uint8_t bias);

void
nokia_cmd_set_disp_mode(const struct nokia5110 *self,
                        enum nokia_display_mode mode);

void
nokia_putc(const struct nokia5110 *self, char c);

void PRINTF(2,3)
nokia_printf(const struct nokia5110 *self, const char *fmt, ...);

void
nokia_puts(const struct nokia5110 *self, const char *s);

void
nokia_goto_xy(const struct nokia5110 *self, uint8_t x, uint8_t y);

void
nokia_set_pixel(const struct nokia5110 *self, uint8_t x, uint8_t y);

void
nokia_clear(const struct nokia5110 *self);

#endif

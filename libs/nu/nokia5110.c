#include "nu/nokia5110.h"
#include "nu/wdt.h"

#define LCD_X     84
#define LCD_Y     48

static const char ASCII[96][5] = {
 {0x00, 0x00, 0x00, 0x00, 0x00} /* 20  (space) */
,{0x00, 0x00, 0x5f, 0x00, 0x00} /* 21 ! */
,{0x00, 0x07, 0x00, 0x07, 0x00} /* 22 " */
,{0x14, 0x7f, 0x14, 0x7f, 0x14} /* 23 # */
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} /* 24 $ */
,{0x23, 0x13, 0x08, 0x64, 0x62} /* 25 % */
,{0x36, 0x49, 0x55, 0x22, 0x50} /* 26 & */
,{0x00, 0x05, 0x03, 0x00, 0x00} /* 27 ' */
,{0x00, 0x1c, 0x22, 0x41, 0x00} /* 28 ( */
,{0x00, 0x41, 0x22, 0x1c, 0x00} /* 29 ) */
,{0x14, 0x08, 0x3e, 0x08, 0x14} /* 2a * */
,{0x08, 0x08, 0x3e, 0x08, 0x08} /* 2b + */
,{0x00, 0x50, 0x30, 0x00, 0x00} /* 2c , */
,{0x08, 0x08, 0x08, 0x08, 0x08} /* 2d - */
,{0x00, 0x60, 0x60, 0x00, 0x00} /* 2e . */
,{0x20, 0x10, 0x08, 0x04, 0x02} /* 2f / */
,{0x3e, 0x51, 0x49, 0x45, 0x3e} /* 30 0 */
,{0x00, 0x42, 0x7f, 0x40, 0x00} /* 31 1 */
,{0x42, 0x61, 0x51, 0x49, 0x46} /* 32 2 */
,{0x21, 0x41, 0x45, 0x4b, 0x31} /* 33 3 */
,{0x18, 0x14, 0x12, 0x7f, 0x10} /* 34 4 */
,{0x27, 0x45, 0x45, 0x45, 0x39} /* 35 5 */
,{0x3c, 0x4a, 0x49, 0x49, 0x30} /* 36 6 */
,{0x01, 0x71, 0x09, 0x05, 0x03} /* 37 7 */
,{0x36, 0x49, 0x49, 0x49, 0x36} /* 38 8 */
,{0x06, 0x49, 0x49, 0x29, 0x1e} /* 39 9 */
,{0x00, 0x36, 0x36, 0x00, 0x00} /* 3a : */
,{0x00, 0x56, 0x36, 0x00, 0x00} /* 3b ; */
,{0x08, 0x14, 0x22, 0x41, 0x00} /* 3c < */
,{0x14, 0x14, 0x14, 0x14, 0x14} /* 3d = */
,{0x00, 0x41, 0x22, 0x14, 0x08} /* 3e > */
,{0x02, 0x01, 0x51, 0x09, 0x06} /* 3f ? */
,{0x32, 0x49, 0x79, 0x41, 0x3e} /* 40 @ */
,{0x7e, 0x11, 0x11, 0x11, 0x7e} /* 41 A */
,{0x7f, 0x49, 0x49, 0x49, 0x36} /* 42 B */
,{0x3e, 0x41, 0x41, 0x41, 0x22} /* 43 C */
,{0x7f, 0x41, 0x41, 0x22, 0x1c} /* 44 D */
,{0x7f, 0x49, 0x49, 0x49, 0x41} /* 45 E */
,{0x7f, 0x09, 0x09, 0x09, 0x01} /* 46 F */
,{0x3e, 0x41, 0x49, 0x49, 0x7a} /* 47 G */
,{0x7f, 0x08, 0x08, 0x08, 0x7f} /* 48 H */
,{0x00, 0x41, 0x7f, 0x41, 0x00} /* 49 I */
,{0x20, 0x40, 0x41, 0x3f, 0x01} /* 4a J */
,{0x7f, 0x08, 0x14, 0x22, 0x41} /* 4b K */
,{0x7f, 0x40, 0x40, 0x40, 0x40} /* 4c L */
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} /* 4d M */
,{0x7f, 0x04, 0x08, 0x10, 0x7f} /* 4e N */
,{0x3e, 0x41, 0x41, 0x41, 0x3e} /* 4f O */
,{0x7f, 0x09, 0x09, 0x09, 0x06} /* 50 P */
,{0x3e, 0x41, 0x51, 0x21, 0x5e} /* 51 Q */
,{0x7f, 0x09, 0x19, 0x29, 0x46} /* 52 R */
,{0x46, 0x49, 0x49, 0x49, 0x31} /* 53 S */
,{0x01, 0x01, 0x7f, 0x01, 0x01} /* 54 T */
,{0x3f, 0x40, 0x40, 0x40, 0x3f} /* 55 U */
,{0x1f, 0x20, 0x40, 0x20, 0x1f} /* 56 V */
,{0x3f, 0x40, 0x38, 0x40, 0x3f} /* 57 W */
,{0x63, 0x14, 0x08, 0x14, 0x63} /* 58 X */
,{0x07, 0x08, 0x70, 0x08, 0x07} /* 59 Y */
,{0x61, 0x51, 0x49, 0x45, 0x43} /* 5a Z */
,{0x00, 0x7f, 0x41, 0x41, 0x00} /* 5b [ */
,{0x02, 0x04, 0x08, 0x10, 0x20} /* 5c � */
,{0x00, 0x41, 0x41, 0x7f, 0x00} /* 5d ] */
,{0x04, 0x02, 0x01, 0x02, 0x04} /* 5e ^ */
,{0x40, 0x40, 0x40, 0x40, 0x40} /* 5f _ */
,{0x00, 0x01, 0x02, 0x04, 0x00} /* 60 ` */
,{0x20, 0x54, 0x54, 0x54, 0x78} /* 61 a */
,{0x7f, 0x48, 0x44, 0x44, 0x38} /* 62 b */
,{0x38, 0x44, 0x44, 0x44, 0x20} /* 63 c */
,{0x38, 0x44, 0x44, 0x48, 0x7f} /* 64 d */
,{0x38, 0x54, 0x54, 0x54, 0x18} /* 65 e */
,{0x08, 0x7e, 0x09, 0x01, 0x02} /* 66 f */
,{0x0c, 0x52, 0x52, 0x52, 0x3e} /* 67 g */
,{0x7f, 0x08, 0x04, 0x04, 0x78} /* 68 h */
,{0x00, 0x44, 0x7d, 0x40, 0x00} /* 69 i */
,{0x20, 0x40, 0x44, 0x3d, 0x00} /* 6a j */
,{0x7f, 0x10, 0x28, 0x44, 0x00} /* 6b k */
,{0x00, 0x41, 0x7f, 0x40, 0x00} /* 6c l */
,{0x7c, 0x04, 0x18, 0x04, 0x78} /* 6d m */
,{0x7c, 0x08, 0x04, 0x04, 0x78} /* 6e n */
,{0x38, 0x44, 0x44, 0x44, 0x38} /* 6f o */
,{0x7c, 0x14, 0x14, 0x14, 0x08} /* 70 p */
,{0x08, 0x14, 0x14, 0x18, 0x7c} /* 71 q */
,{0x7c, 0x08, 0x04, 0x04, 0x08} /* 72 r */
,{0x48, 0x54, 0x54, 0x54, 0x20} /* 73 s */
,{0x04, 0x3f, 0x44, 0x40, 0x20} /* 74 t */
,{0x3c, 0x40, 0x40, 0x20, 0x7c} /* 75 u */
,{0x1c, 0x20, 0x40, 0x20, 0x1c} /* 76 v */
,{0x3c, 0x40, 0x30, 0x40, 0x3c} /* 77 w */
,{0x44, 0x28, 0x10, 0x28, 0x44} /* 78 x */
,{0x0c, 0x50, 0x50, 0x50, 0x3c} /* 79 y */
,{0x44, 0x64, 0x54, 0x4c, 0x44} /* 7a z */
,{0x00, 0x08, 0x36, 0x41, 0x00} /* 7b { */
,{0x00, 0x00, 0x7f, 0x00, 0x00} /* 7c | */
,{0x00, 0x41, 0x36, 0x08, 0x00} /* 7d } */
,{0x10, 0x08, 0x08, 0x10, 0x08} /* 7e ? */
,{0x00, 0x06, 0x09, 0x09, 0x06} /* 7f ? */
}; /* end char ASCII[96][5] */

union nokia5110_instructions {
    union {
        PACKED struct {
            unsigned    use_extended    :1;
            unsigned    addr_mode       :1;
            unsigned    chip_active     :1;
            unsigned    reserved        :2;
            unsigned    fixed           :1;  /* should be 1 */
            unsigned    reserved2       :2;
        } func_set;
    };
    union { /* (useExtended = 0) */
        PACKED struct {
            unsigned    disp_mode       :3;
            unsigned    fixed           :1; /* should be 1 */
            unsigned    reserved        :4;
        } disp_control;
        PACKED struct {
            unsigned    addr            :3;
            unsigned    reserved        :3;
            unsigned    fixed           :1; /* should be 1 */
            unsigned    reserved2       :1;
        } set_ram_y_addr;
        PACKED struct {
            unsigned    addr            :7;
            unsigned    fixed           :1; /* should be 1*/
        } set_ram_x_addr;
    } basic;
    union { /* (useExtended = 1 ) */
        PACKED struct {
            unsigned    temp_coeff      :2;
            unsigned    fixed           :1; /* should be 1 */
            unsigned    reserved        :5;
        } temp_control;
        PACKED struct {
            unsigned    bias            :3;
            unsigned    reserved        :1;
            unsigned    fixed           :1; /* 1 */
            unsigned    reserved2       :3;
        } bias;
        PACKED struct {
            unsigned    vop             :7;
            unsigned    fixed           :1; /* 1 */
        } set_vop;
    } extended;
    byte cmd_byte;
};

#define NOKIA_PREPARE_CMD(a)    do {memset(&a, 0, sizeof(a)); \
                                        a.fixed = 1;} while(0)

#define nokia_drive_reset_low(n)    pin_clear(&((n)->pin_reset))
#define nokia_drive_reset_high(n)   pin_set(&((n)->pin_reset))
#define nokia_drive_dc_low(n)       pin_clear(&((n)->pin_dc))
#define nokia_drive_dc_high(n)      pin_set(&((n)->pin_dc))

static ALWAYSINLINE void
nu_nokia_write_cmd(const struct nokia5110 *n, u8 cmd)
{
    nokia_drive_dc_low(self);
    spi_tx(&(n->spi), &cmd, 1);
}

void
nu_nokia_cmd_func_set(const struct nu_nokia5110 *n,
                enum nu_nokia_cmd_func_set_options opt)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.func_set);
    inst.func_set.addr_mode =
            !!((NU_NOKIA_ADDRESSING_VERTICAL | NU_NOKIA_ADDRESSING_HORIZONTAL) & opt);
    inst.func_set.chip_active =
            !!((NU_NOKIA_POWERDOWN | NU_NOKIA_ACTIVE) & opt);
    inst.func_set.use_extended =
            !!((NU_NOKIA_INSTRUCTIONS_BASIC | NU_NOKIA_INSTRUCTIONS_EXTENDED) & opt);
    nokia_write_cmd(n, inst.cmd_byte);
}

/* NOTE: vop setting is 7-bits wide, going from 0-127 */
void
nu_nokia_cmd_set_vop(const struct nu_nokia5110 *n, u8 vop)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.extended.set_vop);
    inst.extended.set_vop.vop = BITFIELD_CAST(vop,7);  /* 7 bits */
    nokia_write_cmd(n, inst.cmd_byte);
}

/* temp coefficient */
void
nu_nokia_cmd_set_temp_coeff(const struct nokia5110 *n, enum nokia_temp_coeff coeff)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.extended.temp_control);
    inst.extended.temp_control.temp_coeff = coeff;
    nokia_write_cmd(n, inst.cmd_byte);
}

/* bias, which is 3-bits wide ranging from 0-7 */
void
nu_nokia_cmd_set_bias(const struct nokia5110 *n, u8 bias)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.extended.bias);
    inst.extended.bias.bias = BITFIELD_CAST(bias, 3);  /* 3 bits */
    nokia_write_cmd(n, inst.cmd_byte);
}

void
nu_nokia_cmd_set_disp_mode(const struct nokia5110 *n, enum nokia_display_mode mode)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.basic.disp_control);
    inst.basic.disp_control.disp_mode = mode;
    nokia_write_cmd(n, inst.cmd_byte);
}

static ALWAYSINLINE void
write_data(const struct nokia5110 *n, u8 data)
{
    nokia_drive_dc_high(self);
    spi_tx(&(n->spi), &data, 1);
}

void
nu_nokia_clear(const struct nokia5110 *n)
{
    u32 ui;
    for (ui = 0; ui < (LCD_X * LCD_Y) / 8; ++ui) {
        clear_wdt();
        write_data(n, 0x00);
    }
}

/* x should be in the range 0-83 */
static ALWAYSINLINE void
nu_nokia_cmd_set_ram_x_addr(const struct nokia5110 *n, u8 x)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.basic.set_ram_x_addr);
    inst.basic.set_ram_x_addr.addr = BITFIELD_CAST(x, 7); /* 7 bits */
    nokia_write_cmd(n, inst.cmd_byte);
}

/* y should be in the range 0-5 */
static ALWAYSINLINE void
nu_nokia_cmd_set_ram_y_addr(const struct nokia5110 *n, u8 y)
{
    union nokia5110_instructions inst;
    NOKIA_PREPARE_CMD(inst.basic.set_ram_y_addr);
    inst.basic.set_ram_y_addr.addr = BITFIELD_CAST(y, 3);
    nokia_write_cmd(n, inst.cmd_byte);
}

void
nu_nokia_putc(const struct nokia5110 *n, char c)
{
    u32 ui;

    write_data(n, 0x00);
    for (ui = 0; ui < 5; ui++) {
        clear_wdt();
        write_data(n, (BYTE) ASCII[c - 0x20][ui]);
    }

    write_data(n, 0x00);
}

void
nu_nokia_puts(const struct nokia5110 *n, const char *str)
{
    while (*str) {
        clear_wdt();
        nokia_putc(n, *str++);
    }
}

/* x should be in the range 0-83 */
/* y should be in the range 0-5 */
void
nu_nokia_goto_xy(const struct nokia5110 *n, u8 x, u8 y)
{
    nokia_cmd_set_ram_x_addr(n, x);
    nokia_cmd_set_ram_y_addr(n, y);
}

void
nu_nokia_set_pixel(const struct nokia5110 *n, u8 x, u8 y)
{
  /* The LCD has 6 rows, with 8 pixels per  row.
   * 'y_mod' is the row that the pixel is in.
   * 'y_pix' is the pixel in that row we want to enable/disable
   */
  u8 y_mod = (y >> 3);          /* >>3 divides by 8     */
  u8 y_pix = (y-(y_mod << 3));  /* <<3 multiplies by 8  */
  u8 val = 1 << y_pix;

  if (x > 84 || y > 48)
      return;

  /* Write the updated pixel out to the LCD */
  nokia_goto_xy(n, x, y_mod);
  write_data(n, val);
}

void
nu_nokia_printf(const struct nokia5110 *n, const char *fmt, ...)
{
    va_list fmtargs;
    char buffer[72];
    va_start(fmtargs, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, fmtargs);
    va_end(fmtargs);
    nokia_puts(n, buffer);
}

static s32
nu_nokia_report(struct error_reporting_dev *e,
                const char *file, u32 line,
                UNUSED const char *expr,
                UNUSED enum report_priority priority,
                s32 err_num, const char *err_name,
                const char *fmtd_msg)
{
    struct nokia5110 *np = erd_to_nokia5110(e);

    nokia_clear(np);
    nokia_goto_xy(np, 0,0);
    nokia_printf(np, "%.6s:%d", file, line);
    nokia_goto_xy(np, 0, 1);
    nokia_printf(np, "%d(%.18s)", err_num, err_name);
    nokia_goto_xy(np, 0, 3);
    nokia_printf(np, "%.36s", fmtd_msg);

    return 0;
}

const struct nu_vtbl_error_reporting_dev nokia_erd_ops = {
    .report             = &nokia_report,
    .reset_err_state    = NULL,
};

void
nu_nokia_setup(const struct nokia5110 *n)
{
    spi_setup(&(n->spi), 2000000, SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON);

    nu_pin_set_digital_out(&(n->pin_dc));
    nu_pin_set_digital_out(&(n->pin_reset));

    nokia_drive_reset_low(n);
    nokia_drive_reset_high(n);

    nu_nokia_cmd_func_set(n, HORIZONTAL_ADDRESSING, ACTIVE, EXTENDED_INSTRUCTIONS);
    nu_nokia_cmd_set_contrast(n, 57);
    nu_nokia_cmd_set_temp_coeff(n, TEMP_COEFF_0);
    nu_nokia_cmd_set_bias(n, 4);
    nu_nokia_cmd_func_set(n, HORIZONTAL_ADDRESSING, ACTIVE, BASIC_INSTRUCTIONS);
    nu_nokia_cmd_set_disp_mode(n, NORMAL);

    nu_nokia_clear(n);
}
#include "../include/nokia5110.h"

#define LCD_X     84
#define LCD_Y     48

#define NOKIA_PREPARE_CMD(a)    do {memset(&a, 0, sizeof(a)); \
                                        a.fixed = 1;} while(0)

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
,{0x02, 0x04, 0x08, 0x10, 0x20} /* 5c ¥ */
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

union nokia5110Instructions {
    union {
        struct {
            unsigned    useExtended :1;
            unsigned    addrMode    :1;
            unsigned    chipActive  :1;
            unsigned    reserved    :2;
            unsigned    fixed       :1;  /* should be 1 */
            unsigned    reserved2   :2;
        } __attribute__((__packed__)) funcSet ;
    };
    union { /* (useExtended = 0) */
        struct {
            unsigned    dispMode    :3;
            unsigned    fixed       :1; /* should be 1 */
            unsigned    reserved    :4;
        } __attribute__((__packed__)) dispControl;
        struct {
            unsigned    addr        :3;
            unsigned    reserved    :3;
            unsigned    fixed       :1; /* should be 1 */
            unsigned    reserved2   :1;
        } __attribute__((__packed__)) setRamYAddr;
        struct {
            unsigned    addr        :7;
            unsigned    fixed       :1; /* should be 1*/
        } __attribute__((__packed__)) setRamXAddr;
    } basic;
    union { /* (useExtended = 1 ) */
        struct {
            unsigned    tempCoeff   :2;
            unsigned    fixed       :1; /* should be 1 */
            unsigned    reserved    :5;
        } __attribute__((__packed__)) tempControl;
        struct {
            unsigned    bias        :3;
            unsigned    reserved    :1;
            unsigned    fixed       :1; /* 1 */
            unsigned    reserved2   :3;
        } __attribute__((__packed__)) bias;
        struct {
            unsigned    vop         :7;
            unsigned    fixed       :1; /* 1 */
        } __attribute__((__packed__)) setVop;
    } extended;
    BYTE cmdByte;
};

static void
cmdFuncSet(const struct nokia5110 *self, enum nokia_addrMode _addrMode, enum nokia_chipActive active,
            enum nokia_extended useExtended);

static void
cmdSetVop(const struct nokia5110 *self, uint8_t vop);

static inline void
cmdSetContrast(const struct nokia5110 *self, uint8_t contrast);

static void
cmdSetTempCoeff(const struct nokia5110 *self, enum nokia_tempCoeff coeff);

static void
cmdSetBias(const struct nokia5110 *self, uint8_t bias);

static void
cmdSetDispMode(const struct nokia5110 *self, enum nokia_displayMode mode);

static void
writeChar(const struct nokia5110 *self, char c);

static inline void
writeString(const struct nokia5110 *self, const char *str) __attribute__((always_inline));

static void
lcdPrintf(const struct nokia5110 *self, const char *fmt, ...) __attribute__((format(printf,2,3)));

static inline void
gotoXY(const struct nokia5110 *self, uint8_t x, uint8_t y) __attribute__((always_inline));

static void
setPixel(const struct nokia5110 *self, uint8_t x, uint8_t y);

static inline void
clear(const struct nokia5110 *self);

static const struct vtblNokia5110 nokia5110Ops = {
    .cmdFuncSet         = &cmdFuncSet,
    .cmdSetContrast     = &cmdSetContrast,
    .cmdSetVop          = &cmdSetVop,
    .cmdSetTempCoeff    = &cmdSetTempCoeff,
    .cmdSetBias         = &cmdSetBias,
    .cmdSetDispMode     = &cmdSetDispMode,
    
    .writeChar          = &writeChar,
    .writeString        = &writeString,
    .printf             = &lcdPrintf,
    .gotoXY             = &gotoXY,
    .setPixel           = &setPixel,
    .clear              = &clear,
};

static int32_t
nokia_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg);

static const struct vtblError_reporting_dev nokia_erd_ops = {
    .report         = &nokia_report,
    .resetErrState  = NULL,
};

static inline void
driveResetLow(const struct nokia5110 *self) __attribute__((always_inline));

static inline void
driveResetHigh(const struct nokia5110 *self) __attribute__((always_inline));

static int32_t
nokia5110_init(const struct nokia5110 *self);

int32_t
nokia5110_new(struct nokia5110 *self, SpiChannel _chn,
        IoPortId _csPinLtr,      uint32_t _csPinNum,
        IoPortId _resetPinLtr,   uint32_t _resetPinNum,
        IoPortId _dcPinLtr,      uint32_t _dcPinNum)
{
    if (self == NULL)
        return -ENULPTR;

    self->erd.op           = &nokia_erd_ops;
    
    self->op               = &nokia5110Ops;
    self->resetPinLtr      = _resetPinLtr;
    self->resetPinNum      = _resetPinNum;
    self->dcPinLtr         = _dcPinLtr;
    self->dcPinNum         = _dcPinNum;

    SPI_CS_new(&(self->spiPort), _chn, 100000,
                        SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON,
                        AUTO_CS_PIN_ENABLE, _csPinLtr, _csPinNum);

    return nokia5110_init(self);
}

static int32_t
nokia5110_init(const struct nokia5110 *self)
{
    if (self == NULL)
        return -ENULPTR;

    PORTSetPinsDigitalOut(self->resetPinLtr, self->resetPinNum);
    PORTSetPinsDigitalOut(self->dcPinLtr, self->dcPinNum);

    driveResetLow(self);
    driveResetHigh(self);

    cmdFuncSet(self, HORIZONTAL_ADDRESSING, ACTIVE, EXTENDED_INSTRUCTIONS);
    cmdSetContrast(self, 57);
    cmdSetTempCoeff(self, TEMP_COEFF_0);
    cmdSetBias(self, 4);
    cmdFuncSet(self, HORIZONTAL_ADDRESSING, ACTIVE, BASIC_INSTRUCTIONS);
    cmdSetDispMode(self, NORMAL);

    clear(self);

    return 0;
}

static int32_t
nokia_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg)
{
    struct nokia5110 *np = error_reporting_dev_to_nokia5110(self);

    (void)priority;

    if (!self || !file || !expr || !errName || !fmtdMsg)
        return -ENULPTR;

    np->op->clear(np);
    np->op->gotoXY(np, 0,0);
    np->op->printf(np, "%s:%d             ", file, line);
    np->op->gotoXY(np, 0, 3);
    np->op->printf(np, "%d(%s)            ", errNum, errName);
    np->op->gotoXY(np, 0, 4);
    np->op->printf(np, "%s                ", fmtdMsg);

    return 0;
}

static inline void
writeCmd(const struct nokia5110 *self, BYTE cmd) __attribute__((always_inline));

static inline void
writeData(const struct nokia5110 *self, BYTE data);

static void
cmdFuncSet(const struct nokia5110 *self, enum nokia_addrMode _addrMode, enum nokia_chipActive active,
            enum nokia_extended useExtended)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.funcSet);
    inst.funcSet.addrMode = _addrMode;
    inst.funcSet.chipActive = active;
    inst.funcSet.useExtended = useExtended;
    writeCmd(self, inst.cmdByte);
}

/* NOTE: vop setting is 7-bits wide, going from 0-127 */
static void
cmdSetVop(const struct nokia5110 *self, UINT8 vop)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.extended.setVop);
    inst.extended.setVop.vop = BITFIELD_CAST(vop,7);  /* 7 bits */
    writeCmd(self, inst.cmdByte);
}

/* alias for cmdSetVop */
static inline void
cmdSetContrast(const struct nokia5110 *self, UINT8 contrast)
{
    cmdSetVop(self, contrast);
}

/* temp coefficient */
static void
cmdSetTempCoeff(const struct nokia5110 *self, enum nokia_tempCoeff coeff)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.extended.tempControl);
    inst.extended.tempControl.tempCoeff = coeff;
    writeCmd(self, inst.cmdByte);
}

/* bias, which is 3-bits wide ranging from 0-7 */
static void
cmdSetBias(const struct nokia5110 *self, uint8_t bias)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.extended.bias);
    inst.extended.bias.bias = BITFIELD_CAST(bias, 3);  /* 3 bits */
    writeCmd(self, inst.cmdByte);
}

static void
cmdSetDispMode(const struct nokia5110 *self, enum nokia_displayMode mode)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.basic.dispControl);
    inst.basic.dispControl.dispMode = mode;
    writeCmd(self, inst.cmdByte);
}

/* x should be in the range 0-83 */
static void
cmdSetRamXAddr(const struct nokia5110 *self, uint8_t x)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.basic.setRamXAddr);
    inst.basic.setRamXAddr.addr = BITFIELD_CAST(x, 7); /* 7 bits */
    writeCmd(self, inst.cmdByte);
}

/* y should be in the range 0-5 */
static void
cmdSetRamYAddr(const struct nokia5110 *self, uint8_t y)
{
    union nokia5110Instructions inst;
    NOKIA_PREPARE_CMD(inst.basic.setRamYAddr);
    inst.basic.setRamYAddr.addr = BITFIELD_CAST(y, 3);
    writeCmd(self, inst.cmdByte);
}

static void
writeChar(const struct nokia5110 *self, char c)
{
    uint32_t ui;

    writeData(self, 0x00);
    for (ui = 0; ui < 5; ui++)
        writeData(self, (BYTE) ASCII[c - 0x20][ui]);

    writeData(self, 0x00);
}

static inline void
clear(const struct nokia5110 *self)
{
    uint32_t ui;
    for (ui = 0; ui < LCD_X * LCD_Y / 8; ++ui)
        writeData(self, 0x00);
}

static inline void
writeString(const struct nokia5110 *self, const char *str)
{
    while (*str)
        writeChar(self, *str++);
}

/* x should be in the range 0-83 */
/* y should be in the range 0-5 */
static inline void
gotoXY(const struct nokia5110 *self, uint8_t x, uint8_t y)
{
    cmdSetRamXAddr(self, x);
    cmdSetRamYAddr(self, y);
}

static void
setPixel(const struct nokia5110 *self, uint8_t x, uint8_t y)
{
  /* The LCD has 6 rows, with 8 pixels per  row.
   * 'y_mod' is the row that the pixel is in.
   * 'y_pix' is the pixel in that row we want to enable/disable
   */
  uint8_t y_mod = (y >> 3);	/* >>3 divides by 8     */
  uint8_t y_pix = (y-(y_mod << 3)); /* <<3 multiplies by 8  */
  uint8_t val = 1 << y_pix;

  if (x > 84 || y > 48)
      return;

  /* Write the updated pixel out to the LCD */
  gotoXY(self, x, y_mod);
  writeData(self, val);
}

static inline void
driveResetLow(const struct nokia5110 *self)
{
    PORTClearBits(self->resetPinLtr, self->resetPinNum);
}

static inline void
driveResetHigh(const struct nokia5110 *self)
{
    PORTSetBits(self->resetPinLtr, self->resetPinNum);
}

static inline void __attribute__((always_inline))
driveDcLow(const struct nokia5110 *self)
{
    PORTClearBits(self->dcPinLtr, self->dcPinNum);
}

static inline void __attribute__((always_inline))
driveDcHigh(const struct nokia5110 *self)
{
    PORTSetBits(self->dcPinLtr, self->dcPinNum);
}

static inline void
writeCmd(const struct nokia5110 *self, BYTE cmd)
{
    driveDcLow(self);
    self->spiPort.op->tx(&(self->spiPort), &cmd, 1);
}

static inline void
writeData(const struct nokia5110 *self, BYTE data)
{
    driveDcHigh(self);
    self->spiPort.op->tx(&(self->spiPort), &data, 1);
}

static void
lcdPrintf(const struct nokia5110 *self, const char *fmt, ...)
{
    va_list fmtargs;
    char buffer[1024];
    va_start(fmtargs, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, fmtargs);
    va_end(fmtargs);
    writeString(self, buffer);
}

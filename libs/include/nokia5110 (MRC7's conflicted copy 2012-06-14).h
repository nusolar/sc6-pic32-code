#ifndef __NU_NOKIA5110_H
#define __NU_NOKIA5110_H

#include <plib.h>
#include "errorcodes.h"
#include "error_reporting.h"
#include "nu32.h"
#include "spi.h"
#include "utility.h"

/***********************************
*  Schematic:
*  LCD 1-Vcc		----->	NU32v2 3.3V
*  LCD 2-GND		----->	NU32v2 GND
*  LCD 3-SCE		----->	NU32v2 E2
*  LCD 4-RST		----->	NU32v2 E1
*  LCD 5-D/C		----->	NU32v2 E0
*  LCD 6-DNK(MOSI)	----->	NU32v2 F8
*  LCD 7-SCLK		----->	NU32v2 D15
*  LCD 8-LED		----->	330 ohm	----->	5V 
*  (8-LED for backlight only if desired)
************************************/

#define error_reporting_dev_to_nokia5110(erdp)   \
        container_of((erdp), struct nokia5110, erd)

enum nokia_dc {
    DATA,
    COMMAND
};

enum nokia_addrMode {
    HORIZONTAL_ADDRESSING = 0,
    VERTICAL_ADDRESSING = 1
};

enum nokia_chipActive {
    ACTIVE = 0,
    POWERDOWN = 1
};

enum nokia_displayMode {
    BLANK   = 0x00,
    NORMAL  = 0x04,
    ALL_ON  = 0x01,
    INVERSE = 0x05
};

enum nokia_extended {
    BASIC_INSTRUCTIONS = 0,
    EXTENDED_INSTRUCTIONS = 1
};

enum nokia_tempCoeff {
    TEMP_COEFF_0 = 0,
    TEMP_COEFF_1,
    TEMP_COEFF_2,
    TEMP_COEFF_3
};

struct nokia5110 {
    const struct vtblNokia5110  *op;

    struct error_reporting_dev erd;

    struct spiPort spiPort;

    IoPortId resetPinLtr; /* active LOW */
    unsigned int resetPinNum;

    /* input to select either command/address or data input */
    IoPortId dcPinLtr;
    unsigned int dcPinNum;
};

struct vtblNokia5110 {
    void (*cmdFuncSet)      (const struct nokia5110 *self, enum nokia_addrMode _addrMode,
                                enum nokia_chipActive active,
                                enum nokia_extended useExtended);
    void (*cmdSetContrast)  (const struct nokia5110 *self, UINT8 contrast);
    void (*cmdSetVop)       (const struct nokia5110 *self, UINT8 vop);
    void (*cmdSetTempCoeff) (const struct nokia5110 *self, enum nokia_tempCoeff coeff);
    void (*cmdSetBias)      (const struct nokia5110 *self, UINT8 bias);
    void (*cmdSetDispMode)  (const struct nokia5110 *self, enum nokia_displayMode mode);
    void (*writeChar)       (const struct nokia5110 *self, char c);
    void (*printf)          (const struct nokia5110 *self, const char *fmt, ...) __attribute__((format(printf,2,3)));
    void (*writeString)     (const struct nokia5110 *self, const char *str);
    void (*gotoXY)          (const struct nokia5110 *self, UINT8 x, UINT8 y);
    void (*setPixel)        (const struct nokia5110 *self, UINT8 x, UINT8 y);
    void (*clear)           (const struct nokia5110 *self);
};

int32_t
nokia5110_new(struct nokia5110 *self, SpiChannel _chn,
        IoPortId _csPinLtr,      uint32_t _csPinNum,
        IoPortId _resetPinLtr,   uint32_t _resetPinNum,
        IoPortId _dcPinLtr,      uint32_t _dcPinNum);

#endif /* __Nokia5110_NEW_H */

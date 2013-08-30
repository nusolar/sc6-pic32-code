#ifndef NU_PLATFORM_PINCTL_H
#define NU_PLATFORM_PINCTL_H 1

#include "nu/pinctl.h"
#include "nu/types.h"
#include "nu/compiler.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/pinctl.h"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC

struct nu_pin {
    u8 reserved;
};

#define NU_PIN_INIT() {0}

static ALWAYSINLINE void NU_INIT_PIN(struct nu_pin *p) {}

#define NU_PIN_DEFAULT

#define NU_HAVE_PIN_FUNCTIONS 1

static ALWAYSINLINE void nu_pin_set_digital_out(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_set_digital_in(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_set_analog_out(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_set_analog_in(const struct nu_pin *p) {}

static ALWAYSINLINE u32 nu_pin_read(const struct nu_pin *p) { return 0; }

static ALWAYSINLINE void nu_pin_set(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_clear(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_toggle(const struct nu_pin *p) {}

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX
#include <peripheral/ports.h>

typedef struct nu_pin_platform {
    IoPortId ltr;
    u32 num;
} nu_init_pin_platform_args_t;
#define NU_PIN_PLATFORM_INIT(ltr, num) {ltr, num}

static ALWAYSINLINE void
NU_INIT_PIN_PLATFORM(struct nu_pin_platform *p,
        const nu_init_pin_platform_args_t *args)
{
    p->ltr = args->ltr;
    p->num = args->num;
}

/* @FIXME this list is probably wrong...
 * I was accidentally looking at the wrong table...
 */
#if __PIC32_FEATURE_SET__ == 795    /* PIC32MX795F512L */
#  define NU_PIN_DEFAULT {IOPORT_A, BIT_0}
#  define NU_PIN_A0  {IOPORT_A, BIT_0}
#  define NU_PIN_A1  {IOPORT_A, BIT_1}
#  define NU_PIN_A2  {IOPORT_A, BIT_2}
#  define NU_PIN_A3  IOPORT_A, BIT_3
#  define NU_PIN_A4  {IOPORT_A, BIT_4}
#  define NU_PIN_A5  {IOPORT_A, BIT_5}
#  define NU_PIN_A6  IOPORT_A, BIT_6
#  define NU_PIN_A7  IOPORT_A, BIT_7
#  define NU_PIN_A9  IOPORT_A, BIT_9
#  define NU_PIN_A10 IOPORT_A, BIT_10
#  define NU_PIN_A14 IOPORT_A, BIT_14
#  define NU_PIN_A15 IOPORT_A, BIT_15
#  define NU_PIN_B0  IOPORT_B, BIT_0
#  define NU_PIN_B1  IOPORT_B, BIT_1
#  define NU_PIN_B2  IOPORT_B, BIT_2
#  define NU_PIN_B3  IOPORT_B, BIT_3
#  define NU_PIN_B4  IOPORT_B, BIT_4
#  define NU_PIN_B5  IOPORT_B, BIT_5
#  define NU_PIN_B6  IOPORT_B, BIT_6
#  define NU_PIN_B7  IOPORT_B, BIT_7
#  define NU_PIN_B8  IOPORT_B, BIT_8
#  define NU_PIN_B9  IOPORT_B, BIT_9
#  define NU_PIN_B10 IOPORT_B, BIT_10
#  define NU_PIN_B11 IOPORT_B, BIT_11
#  define NU_PIN_B12 IOPORT_B, BIT_12
#  define NU_PIN_B13 IOPORT_B, BIT_13
#  define NU_PIN_B14 IOPORT_B, BIT_14
#  define NU_PIN_B15 IOPORT_B, BIT_15
#  define NU_PIN_C1  IOPORT_C, BIT_1
#  define NU_PIN_C2  IOPORT_C, BIT_2
#  define NU_PIN_C3  IOPORT_C, BIT_3
#  define NU_PIN_C4  IOPORT_C, BIT_4
#  define NU_PIN_C12 IOPORT_C, BIT_12
#  define NU_PIN_C13 IOPORT_C, BIT_13
#  define NU_PIN_C14 IOPORT_C, BIT_14
#  define NU_PIN_C15 IOPORT_C, BIT_15
#  define NU_PIN_D0  IOPORT_D, BIT_0
#  define NU_PIN_D1  IOPORT_D, BIT_1
#  define NU_PIN_D2  IOPORT_D, BIT_2
#  define NU_PIN_D3  IOPORT_D, BIT_3
#  define NU_PIN_D4  IOPORT_D, BIT_4
#  define NU_PIN_D5  IOPORT_D, BIT_5
#  define NU_PIN_D6  IOPORT_D, BIT_6
#  define NU_PIN_D7  IOPORT_D, BIT_7
#  define NU_PIN_D8  IOPORT_D, BIT_8
#  define NU_PIN_D9  IOPORT_D, BIT_9
#  define NU_PIN_D10 IOPORT_D, BIT_10
#  define NU_PIN_D11 IOPORT_D, BIT_11
#  define NU_PIN_D12 IOPORT_D, BIT_12
#  define NU_PIN_D13 IOPORT_D, BIT_13
#  define NU_PIN_D14 IOPORT_D, BIT_14
#  define NU_PIN_D15 IOPORT_D, BIT_15
#  define NU_PIN_E0  IOPORT_E, BIT_0
#  define NU_PIN_E1  IOPORT_E, BIT_1
#  define NU_PIN_E2  IOPORT_E, BIT_2
#  define NU_PIN_E3  IOPORT_E, BIT_3
#  define NU_PIN_E4  IOPORT_E, BIT_4
#  define NU_PIN_E5  IOPORT_E, BIT_5
#  define NU_PIN_E6  IOPORT_E, BIT_6
#  define NU_PIN_E7  IOPORT_E, BIT_7
#  define NU_PIN_E8  IOPORT_E, BIT_8
#  define NU_PIN_E9  IOPORT_E, BIT_9
#  define NU_PIN_F0  IOPORT_F, BIT_0
#  define NU_PIN_F1  IOPORT_F, BIT_1
#  define NU_PIN_F2  IOPORT_F, BIT_2
#  define NU_PIN_F3  IOPORT_F, BIT_3
#  define NU_PIN_F4  IOPORT_F, BIT_4
#  define NU_PIN_F5  IOPORT_F, BIT_5
#  define NU_PIN_F8  IOPORT_F, BIT_8
#  define NU_PIN_F12 IOPORT_F, BIT_12
#  define NU_PIN_F13 IOPORT_F, BIT_13
#  define NU_PIN_G0  IOPORT_G, BIT_0
#  define NU_PIN_G1  IOPORT_G, BIT_1
#  define NU_PIN_G2  IOPORT_G, BIT_2
#  define NU_PIN_G3  IOPORT_G, BIT_3
#  define NU_PIN_G6  IOPORT_G, BIT_6
#  define NU_PIN_G7  IOPORT_G, BIT_7
#  define NU_PIN_G8  IOPORT_G, BIT_8
#  define NU_PIN_G9  IOPORT_G, BIT_9
#  define NU_PIN_G12 {IOPORT_G, BIT_12}
#  define NU_PIN_G13 {IOPORT_G, BIT_13}
#  define NU_PIN_G14 IOPORT_G, BIT_14
#  define NU_PIN_G15 IOPORT_G, BIT_15
#endif

static ALWAYSINLINE void
nu_pin_platform_set_digital_out(const struct nu_pin_platform *p)
{
    PORTSetPinsDigitalOut(p->ltr, p->num);
}

static ALWAYSINLINE void
nu_pin_platform_set_digital_in(const struct nu_pin_platform *p)
{
    PORTSetPinsDigitalIn(p->ltr, p->num);
}

static ALWAYSINLINE void
nu_pin_platform_set_analog_out(const struct nu_pin_platform *p)
{
    PORTSetPinsAnalogOut(p->ltr, p->num);
}

static ALWAYSINLINE void
nu_pin_platform_set_analog_in(const struct nu_pin_platform *p)
{
    PORTSetPinsAnalogIn(p->ltr, p->num);
}

static ALWAYSINLINE u32
nu_pin_platform_read(const struct nu_pin_platform *p)
{
    return PORTReadBits(p->ltr, p->num);
}

static ALWAYSINLINE void
nu_pin_platform_set(const struct nu_pin_platform *p)
{
    PORTSetBits(p->ltr, p->num);
}

static ALWAYSINLINE void
nu_pin_platform_clear(const struct nu_pin_platform *p)
{
    PORTClearBits(p->ltr, p->num);
}

static ALWAYSINLINE void
nu_pin_platform_toggle(const struct nu_pin_platform *p)
{
    PORTToggleBits(p->ltr, p->num);
}

static const struct nu_pin_platform_ops nu_pin_platform_ops = {
    nu_pin_platform_set_digital_out,
    nu_pin_platform_set_digital_in,
    nu_pin_platform_set_analog_out,
    nu_pin_platform_read,
    nu_pin_platform_set,
    nu_pin_platform_clear,
    nu_pin_platform_toggle
};

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_PINCTL_H */

#include <peripheral/ports.h>
#include <stdint.h>
#include "compiler.h"

struct pin {
    IoPortId ltr;
    uint32_t num;
};

#define PIN_INIT(ltr,num)   {(ltr), (num)}
#define PIN_INIX(ltr,num)   PIN_INIT(IOPORT_##ltr, BIT_##num)
# define PIN(name,ltr,num)                  \
    struct pin name = PIN_INIT(ltr, num)
#define PINX(name,ltr,num)  \
    PIN(name, IOPORT_##ltr, BIT_##num)

#define PIN_SET_DIGITAL_OUT(p)  \
    PORTSetPinsDigitalOut((p).ltr, (p).num)

static ALWAYSINLINE void
pin_set_digital_out(const struct pin *p)
{
    if (NULL != p)
        PORTSetPinsDigitalOut(p->ltr, p->num);
}

#define PIN_SET_DIGITAL_IN(p)   \
    PORTSetPinsDigitalIn((p).ltr, (p).num)

static ALWAYSINLINE void
pin_set_digital_in(const struct pin *p)
{
    if (NULL != p)
        PORTSetPinsDigitalIn(p->ltr, p->num);
}

static ALWAYSINLINE void
pin_set_analog_out(const struct pin *p)
{
    if (NULL != p)
        PORTSetPinsAnalogOut(p->ltr, p->num);
}

static ALWAYSINLINE void
pin_set_analog_in(const struct pin *p)
{
    if (NULL != p)
        PORTSetPinsAnalogIn(p->ltr, p->num);
}

#define PIN_READ(p) \
    PORTReadBits((p).ltr, (p).num)

static ALWAYSINLINE uint32_t
pin_read(const struct pin *p)
{
    if (NULL != p)
        return PORTReadBits(p->ltr, p->num);
}

#define PIN_SET(p)  \
    PORTSetBits((p).ltr, (p).num)

static ALWAYSINLINE void
pin_set(const struct pin *p)
{
    if (NULL != p)
        PORTSetBits(p->ltr, p->num);
}

#define PIN_CLEAR(p)    \
    PORTClearBits((p).ltr, (p).num)

static ALWAYSINLINE void
pin_clear(const struct pin *p)
{
    if (NULL != p)
        PORTClearBits(p->ltr, p->num);
}

static ALWAYSINLINE void
pin_toggle(const struct pin *p)
{
    if (NULL != p)
        PORTToggleBits(p->ltr, p->num);
}


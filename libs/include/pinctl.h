#ifndef NU_PINCTL_H
#define NU_PINCTL_H

#include <peripheral/ports.h>
#include "compiler.h"
#include "nu_types.h"

struct pin {
    IoPortId ltr;
    u32 num;
};

#define PIN_INIT(ltr,num)   {(ltr), (num)}
#define PIN_INIX(ltr,num)   PIN_INIT(IOPORT_##ltr, BIT_##num)
#define INIT_PIN(p,_ltr,_num) \
    do{(p)->ltr = (_ltr); (p)->num = _num;}while(0)
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

static ALWAYSINLINE u32
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

#endif

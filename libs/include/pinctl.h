#ifndef NU_PINCTL_H
#define NU_PINCTL_H

#include <peripheral/ports.h>
#include "compiler.h"
#include "nu_types.h"

struct pin {
    IoPortId ltr;
    u32 num;
};

#define PIN_INIT(_ltr,_num)   {.ltr = (_ltr), .num = (_num)}
#define PIN(name, ltr, num) struct pin name = PIN_INIT(ltr, num)

static INLINE void
INIT_PIN(struct pin *p, IoPortId ltr, u32 num)
{
    p->ltr = ltr;
    p->num = num;
}

#define pin_set_digital_out(p)  \
    PORTSetPinsDigitalOut((p)->ltr, (p)->num)
#define pin_set_digital_in(p)   \
    PORTSetPinsDigitalIn((p)->ltr, (p)->num)
#define pin_set_analog_out(p)   \
    PORTSetPinsAnalogOut((p)->ltr, (p)->num)
#define pin_set_analog_in(p)    \
    PORTSetPinsAnalogIn((p)->ltr, (p)->num)
#define pin_read(p) \
    PORTReadBits((p)->ltr, (p)->num)
#define pin_set(p)  \
    PORTSetBits((p)->ltr, (p)->num)
#define pin_clear(p)    \
    PORTClearBits((p)->ltr, (p)->num)
#define pin_toggle(p)   \
    PORTToggleBits((p)->ltr, (p)->num)

#endif

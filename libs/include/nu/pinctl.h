#ifndef NU_PINCTL_H
#define NU_PINCTL_H 1

/**
 * @file
 * Simple pin wrapper
 */

#include <peripheral/ports.h>
#include "compiler.h"
#include "nu_types.h"

struct nu_pin {
    IoPortId ltr;
    u32 num;
};

#define NU_PIN_INIT(ltr,num)   {(ltr), (num)}
#define NU_PIN(name, ltr, num)  \
    struct nu_pin name = NU_PIN_INIT(ltr, num)

static ALWAYSINLINE void
NU_INIT_PIN(struct nu_pin *p, IoPortId ltr, u32 num)
{
    p->ltr = ltr;
    p->num = num;
}

/**
 * Set a nu_pin to Digital Out
 * @param  p [in,out] `struct nu_pin` to set
 */
#define nu_pin_set_digital_out(p)  \
    PORTSetPinsDigitalOut((p)->ltr, (p)->num)
#define nu_pin_set_digital_in(p)   \
    PORTSetPinsDigitalIn((p)->ltr, (p)->num)
#define nu_pin_set_analog_out(p)   \
    PORTSetPinsAnalogOut((p)->ltr, (p)->num)
#define nu_pin_set_analog_in(p)    \
    PORTSetPinsAnalogIn((p)->ltr, (p)->num)
#define nu_pin_read(p) \
    PORTReadBits((p)->ltr, (p)->num)
#define nu_pin_set(p)  \
    PORTSetBits((p)->ltr, (p)->num)
#define nu_pin_clear(p)    \
    PORTClearBits((p)->ltr, (p)->num)
#define nu_pin_toggle(p)   \
    PORTToggleBits((p)->ltr, (p)->num)

#endif

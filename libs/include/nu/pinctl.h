#ifndef NU_PINCTL_H
#define NU_PINCTL_H 1

/**
 * @file
 * Simple pin wrapper
 */

#include "arch.h"
#include "nu_types.h"

/*
 * arch-specific pinctl will provide the following functions/macros:
 * NU_PIN_INIT(...)
 * NU_INIT_PIN(struct nu_pin *p, ...)
 *
 * It should also provide macros in the form NU_PIN_* that contain the
 * set of arguments that define each pin for the architecture.
 *
 * It can also provide inline implementations for the
 * nu_pin_* functions, but must then define NU_HAVE_PIN_FUNCTIONS
 */

#if NU_ARCH == NU_ARCH_PIC32MX
# include "pinctl_pic32mx.h"
#endif

/* PIC32MX example:
 * NU_PIN(main_relay_pin, NU_PIN_A0)
 * will define an nu_pin named main_relay_pin on pin A0.
 */
#define NU_PIN(name, pin) \
    struct nu_pin name = NU_PIN_INIT(pin)

struct nu_pin;

#ifndef NU_HAVE_PIN_FUNCTIONS
void
nu_pin_set_digital_out(const struct nu_pin *p);

void
nu_pin_set_digital_in(const struct nu_pin *p);

void
nu_pin_set_analog_out(const struct nu_pin *p);

void
nu_pin_set_analog_in(const struct nu_pin *p);

u32
nu_pin_read(const struct nu_pin *p);

void
nu_pin_set(const struct nu_pin *p);

void
nu_pin_clear(const struct nu_pin *p);

void
nu_pin_toggle(const struct nu_pin *p);
#endif

#endif

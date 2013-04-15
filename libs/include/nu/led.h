#ifndef NU_LED_H
#define NU_LED_H

/**
 * @file
 * Little wrapper for LEDs
 */

#include "compiler.h"
#include "error_reporting.h"
#include "nu_types.h"
#include "pinctl.h"
#include "utility.h"

struct nu_led {
    struct nu_error_reporting_dev erd;
    struct nu_pin pin;
};

/** Go from a (struct error_reporting_dev *) to the enclosing struct led */
#define nu_erd_to_led(erdp)   \
        container_of((erdp), struct nu_led, erd)

/* declared WEAK because this file is included by nu32.h, but linking against
 * led.c should be optional.
 */
WEAK extern const struct nu_vtbl_error_reporting_dev nu_led_erd_ops;

#define NU_LED_ERD_INIT(ltr, num, min_priority) \
    { \
    NU_ERD_INIT(min_priority, &nu_led_erd_ops), \
    NU_PIN_INIT(ltr, num) \
    }
#define NU_LED_INIT(ltr, num)  \
    NU_LED_ERD_INIT(ltr, num, NU_REP_DEBUG)

#define NU_LED(name, ltr, num) \
    struct nu_led name = NU_LED_INIT(ltr, num)
#define NU_LED_ERD(name, ltr, num, min_priority)   \
    struct nu_led name = NU_LED_ERD_INIT(ltr, num, min_priority)

#define nu_led_off(l)       nu_pin_set(&((l)->pin))
#define nu_led_on(l)        nu_pin_clear(&((l)->pin))
#define nu_led_toggle(l)    nu_pin_toggle(&((l)->pin))

static ALWAYSINLINE void
NU_INIT_LED(struct nu_led *l, IoPortId ltr, unsigned int num)
{
    NU_INIT_PIN(&(l->pin), ltr, num);
}

static INLINE void
nu_led_setup(const struct nu_led *l)
{
    nu_pin_set_digital_out(&(l->pin));
    nu_led_off(l);
}

#endif

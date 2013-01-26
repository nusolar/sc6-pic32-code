#ifndef NU_LED_H
#define NU_LED_H

#include "compiler.h"
#include "error_reporting.h"
#include "nu_types.h"
#include "pinctl.h"
#include "utility.h"

struct led {
    struct error_reporting_dev erd;
    struct pin pin;
};

#define erd_to_led(erdp)   \
        container_of((erdp), struct led, erd)

extern const struct vtbl_error_reporting_dev led_erd_ops;

#define LED_INIT(ltr, num)  { .pin = PIN_INIT(ltr, num) }
#define LED(name, ltr, num) \
    struct led name = LED_INIT(ltr, num)
#define LED_ERD_INIT(ltr, num, min_priority)        \
    {                                               \
    .erd = ERD_INIT(min_priority, &led_erd_ops),    \
    .pin = PIN_INIT(ltr, num)                       \
    }
#define LED_ERD(name, ltr, num, min_priority)   \
    struct led name = LED_ERD_INIT(ltr, num, min_priority)

#define led_off(l)      pin_set(&((l)->pin))
#define led_on(l)       pin_clear(&((l)->pin))
#define led_toggle(l)   pin_toggle(&((l)->pin))

static INLINE void
led_setup(const struct led *l)
{
    pin_set_digital_out(&(l->pin));
    led_off(l);
}

#endif

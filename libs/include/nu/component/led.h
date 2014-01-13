#ifndef NU_LED_H
#define NU_LED_H 1

#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include "nu/types.h"
#include "nu/pinctl.h"
#include "nu/utility/data.h"

struct nu_led {
    struct nu_error_reporting_dev erd;
    struct nu_pin pin;
};
#define NU_LED_INIT(erd, pin) {erd, pin}
#define NU_LED_ERD_INIT(priority)  \
    NU_ERD_INIT(priority, &nu_led_erd_ops)
#define NU_LED(name, erd, pin) \
    struct nu_led name = NU_LED_INIT(erd, pin)

/** Go from a (struct error_reporting_dev *) to the enclosing struct led */
#define nu_erd_to_led(erdp)   \
    container_of((erdp), struct nu_led, erd)

extern const struct nu_vtbl_error_reporting_dev nu_led_erd_ops;

#define nu_led_off(l)       nu_pin_set(&((l)->pin))
#define nu_led_on(l)        nu_pin_clear(&((l)->pin))
#define nu_led_toggle(l)    nu_pin_toggle(&((l)->pin))

static INLINE void
nu_led_setup(const struct nu_led *l)
{
    nu_pin_set_digital_out(&(l->pin));
    nu_led_off(l);
}

#endif

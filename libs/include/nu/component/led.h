#ifndef NU_LED_H
#define NU_LED_H 1

#include "nu/peripheral/pinctl.h"
#include "nu/utility/data.h"
#include "nu/error_reporting.h"
#include "nu/types.h"
#include "nu/compiler.h"

struct nu__Led {
    struct nu__ErrorReportingDev erd;
    struct nu__Pin pin;
};
#define NU_LED_INIT(erd, pin) {erd, pin}
#define NU_LED_ERD_INIT(priority)  \
    NU_ERD_INIT(priority, &nu__Led__erd_ops)
#define NU_LED(name, erd, pin) \
    struct nu__Led name = NU_LED_INIT(erd, pin)

/** Go from a (struct error_reporting_dev *) to the enclosing struct led */
#define nu_erd_to_led(erdp)   \
    container_of((erdp), struct nu__Led, erd)

extern const struct nu__ErrorReportingDev__Vtbl nu__Led__erd_ops;

#define nu__Led__off(l)       nu__Pin__set(&((l)->pin))
#define nu__Led__on(l)        nu__Pin__clear(&((l)->pin))
#define nu__Led__toggle(l)    nu__Pin__toggle(&((l)->pin))

static INLINE void
nu__Led__setup(const struct nu__Led *l)
{
    nu__Pin__set_digital_out(&(l->pin));
    nu__Led__off(l);
}

#endif

#ifndef NU_BUTTON_H
#define NU_BUTTON_H

#include "arith.h"
#include "pinctl.h"

struct btn {
    struct nu_pin pin;
    u8 debounce;
    u8 debounce_max;
    u8 thresh;
};

#define BTN_INIT(ltr,num,_debounce_max,_thresh) \
    {                                           \
    .pin = NU_PIN_INIT(ltr,num),                   \
    .debounce = 0,                              \
    .debounce_max = (_debounce_max),            \
    .thresh = (_thresh)                         \
    }
#define BTN(name, ltr, num, debounce_max, thresh)   \
    struct btn name = BTN_INIT(ltr, num, debounce_max, thresh)

#define btn_pressed(b)  ((b)->debounce >= (b)->thresh)

/**
 * Add or subtract 1 to b->debounce, depending on whether b->pin is HIGH.
 * @param  b [in, out] the `struct btn` button to be updated
 */
#define btn_update(b)                                               \
    (b)->debounce =                                                 \
        clamp((b)->debounce + nu_pin_read(&((b)->pin)) ? 1 : -1,       \
            0,                                                      \
            min(__MAX(typeof((b)->debounce)), (b)->debounce_max))

#endif


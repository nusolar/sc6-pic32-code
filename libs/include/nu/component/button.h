#ifndef NU_BUTTON_H
#define NU_BUTTON_H 1

#include "nu/peripheral/pinctl.h"
#include "nu/utility/arith.h"

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

/**
 * compute whether button is pressed
 * @param  b [in, out] `struct btn` to check
 */
#define btn_pressed(b)  ((b)->debounce >= (b)->thresh)

/**
 * Add or subtract 1 to b->debounce, if b->pin is HIGH or LOW
 * @param  b [in, out] the `struct btn` button to be updated
 * @note If clock lags, 10 cycles needed until button normalizes!
 */
#define btn_update(b)                                               \
    (b)->debounce =                                                 \
        clamp((b)->debounce + nu_pin_read(&((b)->pin)) ? 1 : -1,       \
            0,                                                      \
            min(__MAX(typeof((b)->debounce)), (b)->debounce_max))

#endif


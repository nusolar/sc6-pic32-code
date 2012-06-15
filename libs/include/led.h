#ifndef __NU_LED
#define __NU_LED

#include <plib.h>
#include <stdint.h>
#include "errorcodes.h"
#include "error_reporting.h"
#include "utility.h"

#define error_reporting_dev_to_led(erdp)   \
        container_of((erdp), struct led, erd)

struct led {
    struct error_reporting_dev erd;
    const struct vtblLed *op;
    uint32_t ledPinNum;
    IoPortId ledPinLtr;
};

struct vtblLed {
    int32_t (*on)      (const struct led *self);
    int32_t (*off)     (const struct led *self);
    int32_t (*toggle)  (const struct led *self);
};

int32_t
led_new(struct led *self, IoPortId ledPinLtr, uint32_t ledPinNum);

#endif

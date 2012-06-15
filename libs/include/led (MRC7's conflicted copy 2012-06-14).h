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
    const struct vtblLed *op;
    
    struct error_reporting_dev erd;

    IoPortId ledPinLtr;
    uint32_t ledPinNum;
};

struct vtblLed {
    int32_t (*on)      (const struct led *self);
    int32_t (*off)     (const struct led *self);
    int32_t (*toggle)  (const struct led *self);
};

int32_t
led_new(struct led *self, IoPortId ledPinLtr, uint32_t ledPinNum);

#endif

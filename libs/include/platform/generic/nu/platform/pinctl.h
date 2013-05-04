#ifndef NU_PINCTL_GENERIC_H
#define NU_PINCTL_GENERIC_H 1

#include "nu/compiler.h"
#include "nu/types.h"

struct nu_pin {
    u8 reserved;
};

#define NU_PIN_INIT() {0}

static ALWAYSINLINE void NU_INIT_PIN(struct nu_pin *p) {}

#define NU_PIN_DEFAULT

#define NU_HAVE_PIN_FUNCTIONS 1

static ALWAYSINLINE void nu_pin_set_digital_out(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_set_digital_in(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_set_analog_out(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_set_analog_in(const struct nu_pin *p) {}

static ALWAYSINLINE u32 nu_pin_read(const struct nu_pin *p) { return 0; }

static ALWAYSINLINE void nu_pin_set(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_clear(const struct nu_pin *p) {}

static ALWAYSINLINE void nu_pin_toggle(const struct nu_pin *p) {}

#endif

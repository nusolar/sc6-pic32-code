#ifndef NU_PINCTL_H
#define NU_PINCTL_H 1

#include "nu/types.h"

/*
 * NU_PIN_PLATFORM_INIT
 * NU_INIT_PIN_PLATFORM
 * nu_init_pin_platform_args_t
 * (optional) nu_init_pin_platform_defaults
 * nu_pin_platform_setup
 * nu_pin_platform_setup_args_t
 * (optional) nu_pin_platform_setup_defaults
 */
#include "nu/platform/pinctl.h"

struct nu_pin {
    struct nu_pin_platform platform;
};
#define NU_PIN_INIT(platform) {platform}
#define NU_PIN(name, platform) \
    struct nu_pin name = NU_PIN_INIT(platform)

static ALWAYSINLINE void
NU_INIT_PIN(struct nu_pin *p, const nu_init_pin_platform_args_t *args)
{
    NU_INIT_PIN_PLATFORM(&(p->platform), args);
}

static ALWAYSINLINE void
nu_pin_set_digital_out(const struct nu_pin *p)
{
    nu_pin_platform_set_digital_out(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_set_digital_in(const struct nu_pin *p)
{
    nu_pin_platform_set_digital_in(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_set_analog_out(const struct nu_pin *p)
{
    nu_pin_platform_set_analog_out(&(p->platform));
}

static ALWAYSINLINE u32
nu_pin_read(const struct nu_pin *p)
{
    return nu_pin_platform_read(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_set(const struct nu_pin *p)
{
    nu_pin_platform_set(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_clear(const struct nu_pin *p)
{
    nu_pin_platform_clear(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_toggle(const struct nu_pin *p)
{
    nu_pin_platform_toggle(&(p->platform));
}

#endif

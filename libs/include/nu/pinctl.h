#ifndef NU_PINCTL_H
#define NU_PINCTL_H 1

#include "nu/types.h"

struct nu_pin_platform;

struct nu_pin_platform_ops {
    void (*set_digital_out) (const struct nu_pin_platform *p);
    void (*set_digital_in)  (const struct nu_pin_platform *p);
    void (*set_analog_out)  (const struct nu_pin_platform *p);
    u32  (*read)            (const struct nu_pin_platform *p);
    void (*set)             (const struct nu_pin_platform *p);
    void (*clear)           (const struct nu_pin_platform *p);
    void (*toggle)          (const struct nu_pin_platform *p);
};

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
    if (nu_pin_platform_ops.set_digital_out)
        nu_pin_platform_ops.set_digital_out(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_set_digital_in(const struct nu_pin *p)
{
    if (nu_pin_platform_ops.set_digital_in)
        nu_pin_platform_ops.set_digital_in(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_set_analog_out(const struct nu_pin *p)
{
    if (nu_pin_platform_ops.set_analog_out)
        nu_pin_platform_ops.set_analog_out(&(p->platform));
}

static ALWAYSINLINE u32
nu_pin_read(const struct nu_pin *p)
{
    return nu_pin_platform_ops.set_analog_out ?
        nu_pin_platform_ops.read(&(p->platform)) : 0;
}

static ALWAYSINLINE void
nu_pin_set(const struct nu_pin *p)
{
    if (nu_pin_platform_ops.set)
        nu_pin_platform_ops.set(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_clear(const struct nu_pin *p)
{
    if (nu_pin_platform_ops.clear)
        nu_pin_platform_ops.clear(&(p->platform));
}

static ALWAYSINLINE void
nu_pin_toggle(const struct nu_pin *p)
{
    if (nu_pin_platform_ops.toggle)
        nu_pin_platform_ops.toggle(&(p->platform));
}

#endif

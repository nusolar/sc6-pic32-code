#ifndef NU_PINCTL_H
#define NU_PINCTL_H 1

#include "nu/types.h"

struct nu__Pin__Platform;

struct nu__Pin__PlatformOps {
    void (*set_digital_out) (const struct nu__Pin__Platform *p);
    void (*set_digital_in)  (const struct nu__Pin__Platform *p);
    void (*set_analog_out)  (const struct nu__Pin__Platform *p);
    u32  (*read)            (const struct nu__Pin__Platform *p);
    void (*set)             (const struct nu__Pin__Platform *p);
    void (*clear)           (const struct nu__Pin__Platform *p);
    void (*toggle)          (const struct nu__Pin__Platform *p);
};

/*
 * NU_PIN_PLATFORM_INIT
 * NU_INIT_PIN_PLATFORM
 * nu__Pin__PlatformInitArgs_t
 * (optional) nu_init_pin_platform_defaults
 * nu__Pin__platform_setup
 * nu__Pin__platform_setup_args_t
 * (optional) nu__Pin__platform_setup_defaults
 */
#include "nu/platform/pinctl.h"

struct nu__Pin {
    struct nu__Pin__Platform platform;
};
#define NU_PIN_INIT(platform) {platform}
#define NU_PIN(name, platform) \
    struct nu__Pin name = NU_PIN_INIT(platform)

static ALWAYSINLINE void
NU_INIT_PIN(struct nu__Pin *p, const nu__Pin__PlatformInitArgs_t *args)
{
    NU_INIT_PIN_PLATFORM(&(p->platform), args);
}

static ALWAYSINLINE void
nu__Pin__set_digital_out(const struct nu__Pin *p)
{
    if (nu__Pin__platform_ops.set_digital_out)
        nu__Pin__platform_ops.set_digital_out(&(p->platform));
}

static ALWAYSINLINE void
nu__Pin__set_digital_in(const struct nu__Pin *p)
{
    if (nu__Pin__platform_ops.set_digital_in)
        nu__Pin__platform_ops.set_digital_in(&(p->platform));
}

static ALWAYSINLINE void
nu__Pin__set_analog_out(const struct nu__Pin *p)
{
    if (nu__Pin__platform_ops.set_analog_out)
        nu__Pin__platform_ops.set_analog_out(&(p->platform));
}

static ALWAYSINLINE u32
nu__Pin__read(const struct nu__Pin *p)
{
    return nu__Pin__platform_ops.set_analog_out ?
        nu__Pin__platform_ops.read(&(p->platform)) : 0;
}

static ALWAYSINLINE void
nu__Pin__set(const struct nu__Pin *p)
{
    if (nu__Pin__platform_ops.set)
        nu__Pin__platform_ops.set(&(p->platform));
}

static ALWAYSINLINE void
nu__Pin__clear(const struct nu__Pin *p)
{
    if (nu__Pin__platform_ops.clear)
        nu__Pin__platform_ops.clear(&(p->platform));
}

static ALWAYSINLINE void
nu__Pin__toggle(const struct nu__Pin *p)
{
    if (nu__Pin__platform_ops.toggle)
        nu__Pin__platform_ops.toggle(&(p->platform));
}

#endif

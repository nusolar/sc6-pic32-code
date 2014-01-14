#ifndef NU_SERIAL_H
#define NU_SERIAL_H 1

#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include "nu/utility/data.h"

struct nu__Serial__Platform;
struct nu__Serial__PlatformSetupArgs;

struct nu__Serial__PlatformOps {
    void (*setup)   (struct nu__Serial__Platform *p, u32 baud,
                        const struct nu__Serial__PlatformSetupArgs *args);
    s32 (*putchar)  (const struct nu__Serial__Platform *p, s32 c);
    s32 (*getchar)  (const struct nu__Serial__Platform *p);
};

/* NU_SERIAL_PLATFORM_INIT
 * NU_INIT_SERIAL_PLATFORM
 * nu_init_serial_platform_args_t */
#include "nu/platform/serial.h"

struct nu__Serial {
    struct nu__ErrorReportingDev erd;
    struct nu__Serial__Platform platform;
};
#define nu_erd_to_serial(erdp) \
    container_of((erdp), struct nu__Serial, erd)

extern const struct nu__ErrorReportingDev__Vtbl nu__Serial__erd_ops;

#define NU_ERD_SERIAL_INIT(min_priority) \
    NU_ERD_INIT(min_priority, nu__Serial__erd_ops)
#define NU_SERIAL_INIT(erd, platform) {erd, platform}
#define NU_SERIAL(name, erd, platform) \
    struct nu__Serial name = NU_SERIAL_INIT(erd, platform)

static ALWAYSINLINE void
NU_INIT_SERIAL(struct nu__Serial *s, const nu_init_serial_platform_args_t *args)
{
    NU_INIT_SERIAL_PLATFORM(&(s->platform), args);
}

void
nu__Serial__setup(struct nu__Serial *s, u32 baud,
	const struct nu__Serial__PlatformSetupArgs *arg);

void
nu__Serial__puts(const struct nu__Serial *s, const char *str);

void PRINTF(2, 3)
nu__Serial__printf(const struct nu__Serial *s, const char *fmt, ...);

size_t
nu__Serial__rx(const struct nu__Serial *s, void *dst, size_t n);

size_t
nu__Serial__tx(const struct nu__Serial *s, const void *src, size_t n);

#endif

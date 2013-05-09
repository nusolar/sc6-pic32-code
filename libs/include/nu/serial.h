#ifndef NU_SERIAL_H
#define NU_SERIAL_H 1

#include "nu/compiler.h"
#include "nu/error_reporting.h"
#include "nu/utility/data.h"
/* NU_SERIAL_PLATFORM_INIT
 * NU_INIT_SERIAL_PLATFORM
 * nu_init_serial_platform_args_t */
#include "nu/platform/serial.h"

struct nu_serial {
    struct nu_error_reporting_dev erd;
    struct nu_serial_platform platform;
};
#define nu_erd_to_serial(erdp) \
    container_of((erdp), struct nu_serial, erd)

extern const struct nu_vtbl_error_reporting_dev nu_serial_erd_ops;

#define NU_ERD_SERIAL_INIT(min_priority) \
    NU_ERD_INIT(min_priority, nu_serial_erd_ops)
#define NU_SERIAL_INIT(erd, platform) {erd, platform}
#define NU_SERIAL(name, erd, platform) \
    struct nu_serial name = NU_SERIAL_INIT(erd, platform)

static ALWAYSINLINE void
NU_INIT_SERIAL(struct nu_serial *s, const nu_init_serial_platform_args_t *args)
{
    NU_INIT_SERIAL_PLATFORM(&(s->platform), args);
}

void
nu_serial_setup(struct nu_serial *s, u32 baud,
	const struct nu_serial_platform_setup_args *arg);

void
nu_serial_puts(const struct nu_serial *s, const char *str);

void PRINTF(2, 3)
nu_serial_printf(const struct nu_serial *s, const char *fmt, ...);

size_t
nu_serial_rx(const struct nu_serial *s, void *dst, size_t n);

size_t
nu_serial_tx(const struct nu_serial *s, const void *src, size_t n);

#endif

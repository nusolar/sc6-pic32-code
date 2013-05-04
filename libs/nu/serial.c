#include "nu/serial.h"
#include "nu/errorcodes.h"
#include "nu/utility.h"
#include "nu/wdt.h"
#include "nu/platform/nu32.h"
#include <alloca.h>
#include <stdarg.h>
#include <stdio.h>

static COLD void
nu_serial_report(struct nu_error_reporting_dev *e,
                    const char *file, const char *func, u32 line,
                    const char *expr,
                    UNUSED enum nu_report_priority priority,
                    const char *msg)
{
    struct nu_serial *s = nu_erd_to_serial(e);

    nu_serial_printf(s,
        "%s:%s:%d:%s\r\n"
        "\t%s\r\n",
        file, func, line, expr,
        msg);

    return;
}

void nu_serial_setup(struct nu_serial *s, u32 baud,
        struct nu_serial_platform_setup_args *arg)
{
    nu_serial_platform_setup(&(s->platform), baud,
	    arg);
}

const struct nu_vtbl_error_reporting_dev nu_serial_erd_ops = {
    &nu_serial_report,
    NULL
};

void
nu_serial_printf(const struct nu_serial *s, const char *fmt, ...)
{
    char *buf;
    int res;
    va_list fmtargs;
    va_start(fmtargs, fmt);
    if (likely((res = vsnprintf(NULL, 0, fmt, fmtargs)) >= 0)) {
        buf = (char *) alloca((size_t)res);
        vsprintf(buf, fmt, fmtargs);
        nu_serial_tx(s, buf, (size_t)res);
    }
    va_end(fmtargs);
}

void
nu_serial_puts(const struct nu_serial *s, const char *str)
{
	nu_serial_tx(s, str, strlen(str));
}

size_t
nu_serial_rx(const struct nu_serial *s, void *dst, size_t n)
{
    return nu_serial_platform_rx(&(s->platform), dst, n);
}

size_t
nu_serial_tx(const struct nu_serial *s, const void *src, size_t n)
{
    return nu_serial_platform_tx(&(s->platform), src, n);
}
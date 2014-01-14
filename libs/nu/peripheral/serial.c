#include "nu/peripheral/serial.h"
#include "nu/wdt.h"
#include <alloca.h>
#include <stdarg.h>
#include <stdio.h>

#undef getchar
#undef putchar

static COLD void
nu_serial_report(struct nu_error_reporting_dev *e,
                    const char *file, const char *func, u32 line,
                    const char *expr,
                    enum nu_report_priority priority,
                    const char *msg)
{
    struct nu_serial *s = nu_erd_to_serial(e);
    nu_serial_printf(s,
        "<%d/%s> %s:%s:%d:%s\r\n"
        "\t%s\r\n",
        priority, nu_report_priority_str[priority], file, func, line, expr,
        msg);
}

const struct nu_vtbl_error_reporting_dev nu_serial_erd_ops = {
    &nu_serial_report,
    NULL
};

void nu_serial_setup(struct nu_serial *s, u32 baud,
        const struct nu_serial_platform_setup_args *args)
{
    if (nu_serial_platform_ops.setup)
        nu_serial_platform_ops.setup(&(s->platform), baud, args);
}

size_t
nu_serial_rx(const struct nu_serial *s, void *dst, size_t n)
{
    size_t ui;
    if (!nu_serial_platform_ops.getchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        s32 c;
        nu_wdt_clear();
        if ((c = nu_serial_platform_ops.getchar(&(s->platform))) < 0)
            return ui;
        ((u8 *)dst)[ui] = (u8)c;
    }
    return ui;
}

size_t
nu_serial_tx(const struct nu_serial *s, const void *src, size_t n)
{
    size_t ui;
    if (!nu_serial_platform_ops.putchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        nu_wdt_clear();
        if (nu_serial_platform_ops.putchar(&(s->platform), ((const u8 *)src)[ui]) < 0)
            break;
    }
    return ui;
}

void
nu_serial_puts(const struct nu_serial *s, const char *str)
{
	nu_serial_tx(s, str, strlen(str));
    nu_serial_tx(s, "\n", 1);
}

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

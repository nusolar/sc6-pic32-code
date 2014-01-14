#include "nu/peripheral/serial.h"
#include "nu/wdt.h"
#include <alloca.h>
#include <stdarg.h>
#include <stdio.h>

#undef getchar
#undef putchar

static COLD void
nu__Serial__report(struct nu__ErrorReportingDev *e,
                    const char *file, const char *func, u32 line,
                    const char *expr,
                    enum nu_report_priority priority,
                    const char *msg)
{
    struct nu__Serial *s = nu_erd_to_serial(e);
    nu__Serial__printf(s,
        "<%d/%s> %s:%s:%d:%s\r\n"
        "\t%s\r\n",
        priority, nu_report_priority_str[priority], file, func, line, expr,
        msg);
}

const struct nu__ErrorReportingDev__Vtbl nu__Serial__erd_ops = {
    &nu__Serial__report,
    NULL
};

void nu__Serial__setup(struct nu__Serial *s, u32 baud,
        const struct nu__Serial__PlatformSetupArgs *args)
{
    if (nu__Serial__platform_ops.setup)
        nu__Serial__platform_ops.setup(&(s->platform), baud, args);
}

size_t
nu__Serial__rx(const struct nu__Serial *s, void *dst, size_t n)
{
    size_t ui;
    if (!nu__Serial__platform_ops.getchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        s32 c;
        nu__WDT__clear();
        if ((c = nu__Serial__platform_ops.getchar(&(s->platform))) < 0)
            return ui;
        ((u8 *)dst)[ui] = (u8)c;
    }
    return ui;
}

size_t
nu__Serial__tx(const struct nu__Serial *s, const void *src, size_t n)
{
    size_t ui;
    if (!nu__Serial__platform_ops.putchar)
        return 0;
    for (ui = 0; ui < n; ++ui) {
        nu__WDT__clear();
        if (nu__Serial__platform_ops.putchar(&(s->platform), ((const u8 *)src)[ui]) < 0)
            break;
    }
    return ui;
}

void
nu__Serial__puts(const struct nu__Serial *s, const char *str)
{
	nu__Serial__tx(s, str, strlen(str));
    nu__Serial__tx(s, "\n", 1);
}

void
nu__Serial__printf(const struct nu__Serial *s, const char *fmt, ...)
{
    char *buf;
    int res;
    va_list fmtargs;
    va_start(fmtargs, fmt);
    if (likely((res = vsnprintf(NULL, 0, fmt, fmtargs)) >= 0)) {
        buf = (char *) alloca((size_t)res);
        vsprintf(buf, fmt, fmtargs);
        nu__Serial__tx(s, buf, (size_t)res);
    }
    va_end(fmtargs);
}

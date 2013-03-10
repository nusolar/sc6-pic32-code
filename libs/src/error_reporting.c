#include "error_reporting.h"
#include "list.h"
#include "lock.h"
#include "utility.h"
#include "wdt.h"
#include <alloca.h>
#include <stdarg.h>
#include <stdio.h>
#include <xc.h>

static LIST_HEAD(erd_list);

static nu_reporting_prehook  warn_prehook   = NULL;
static nu_reporting_posthook warn_posthook  = NULL;
static nu_reporting_prehook  panic_prehook  = NULL;
static nu_reporting_posthook panic_posthook = NULL;

MAYBE_UNUSED s32 _nu_ret_warn_on;

static volatile u8 warn_recurse_lock = 0;

static bool
warn_enter(void)
{
    return nu_lock_acquire(&warn_recurse_lock);
}

static void
warn_exit(void)
{
    nu_lock_release(&warn_recurse_lock);
}

COLD void
nu_reporting_register_warn_prehook(nu_reporting_prehook func)
{
    warn_prehook = func;
}

COLD void
nu_reporting_register_warn_posthook(nu_reporting_posthook func)
{
    warn_posthook = func;
}

COLD void
nu_reporting_register_panic_prehook(nu_reporting_prehook func)
{
    panic_prehook = func;
}

COLD void
nu_reporting_register_panic_posthook(nu_reporting_posthook func)
{
    panic_posthook = func;
}

void
nu_reporting_register(struct nu_error_reporting_dev *erd)
{
     list_add(&(erd->list), &erd_list);
}

void
nu_reporting_unregister(struct nu_error_reporting_dev *erd)
{
    list_del(&(erd->list));
}

COLD void PRINTF(6,7)
nu_warn(const char *file, const char *func, u32 line, const char *expr,
    enum nu_report_priority priority, const char *fmt, ...)
{
    const char *blank = "";
    char *buf;
    const char *fmtd_msg;
    s32 n;
    va_list fmtargs;
    struct nu_error_reporting_dev *pos;

    if (!warn_enter())
        return;

    if (!file)
        file = blank;
    if (!func)
        func = blank;
    if (!expr)
        expr = blank;

    va_start(fmtargs, fmt);

    if (warn_prehook)
        warn_prehook(file, func, line, expr, priority, fmt, fmtargs);

    if ((n = vsnprintf(NULL, 0, fmt, fmtargs)) >= 0) {
        buf = (char *)alloca((size_t)n);
        vsprintf(buf, fmt, fmtargs);
        va_end(fmtargs);
        fmtd_msg = buf;
    } else {
        fmtd_msg = "No msg...vsnprintf() error inside reporting library";
    }

    list_for_each_entry(pos, &erd_list, list) {
        clear_wdt();
        if (pos->min_priority <= priority)
            pos->op->report(pos, file, func, line, expr, priority, fmtd_msg);
    }

    if (warn_posthook)
        warn_posthook(file, func, line, expr, priority, fmtd_msg);

    warn_exit();
}

COLD NORETURN void PRINTF(5,6)
nu_panic(const char *file, const char *func, u32 line, const char *expr,
    const char *fmt, ...)
{
    const char *blank = "";
    char *buf;
    const char *fmtd_msg;
    s32 n;
    va_list fmtargs;
    struct nu_error_reporting_dev *pos;

    warn_enter();

    disable_clear_wdt();

    if (!file)
        file = blank;
    if (!func)
        func = blank;
    if (!expr)
        expr = blank;

    va_start(fmtargs, fmt);

    if (panic_prehook)
        panic_prehook(file, func, line, expr, NU_REP_PANIC, fmt, fmtargs);

    if ((n = vsnprintf(NULL, 0, fmt, fmtargs)) >= 0) {
        buf = (char *)alloca((size_t)n);
        vsprintf(buf, fmt, fmtargs);
        va_end(fmtargs);
        fmtd_msg = buf;
    } else {
        fmtd_msg = "No msg...vsnprintf() error inside reporting library";
    }

    list_for_each_entry(pos, &erd_list, list)
        pos->op->report(pos, file, func, line, expr, NU_REP_PANIC, fmtd_msg);

    if (panic_posthook)
        panic_posthook(file, func, line, expr, NU_REP_PANIC, fmtd_msg);

    while (1)
        Nop();
}

void
nu_reporting_clear(enum nu_report_priority max_priority)
{
    struct nu_error_reporting_dev *pos;
    list_for_each_entry(pos, &erd_list, list)
        if (pos->min_priority <= max_priority)
            pos->op->reset_err_state(pos);
}

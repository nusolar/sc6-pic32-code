#include "nu/error_reporting.h"
#include "nu/list.h"
#include "nu/lock.h"
#include "nu/utility.h"
#include "nu/wdt.h"
#include <alloca.h>
#include <stdio.h>

const char * const nu_report_priority_str[] = {
#define NU_PRIORITY_AS_STRING(x)   #x,
    NU_REPORT_PRIORITIES(NU_PRIORITY_AS_STRING)
};

static LIST_HEAD(erd_list);

static nu_reporting_prehook     warn_prehook   = NULL;
static nu_reporting_posthook    warn_posthook  = NULL;
static nu_reporting_prehook     panic_prehook  = NULL;
static nu_reporting_posthook    panic_posthook = NULL;

static volatile u8 warn_recurse_lock = 0;

static ALWAYSINLINE bool
warn_enter(void)
{
    return nu_lock_acquired(&warn_recurse_lock);
}

static ALWAYSINLINE void
warn_exit(void)
{
    nu_lock_release(&warn_recurse_lock);
}

static ALWAYSINLINE bool
panic_enter(void)
{
    static volatile u8 panic_lock = 0;
    return nu_lock_acquired(&panic_lock);
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

void
nu_reporting_clear(enum nu_report_priority max_priority)
{
    struct nu_error_reporting_dev *pos;
    list_for_each_entry(pos, &erd_list, list)
        if (pos->min_priority <= max_priority)
            pos->op->reset_err_state(pos);
}

COLD void PRINTF(6,0)
nu_vwarn(char const * file, char const * func, const u32 line,
    char const * expr, const enum nu_report_priority priority,
    char const * fmt, va_list vl)
{
    const char *msg = NULL;
    s32 siz;
    struct nu_error_reporting_dev *pos = NULL;

    if (!warn_enter())
        return;

    file = file ? file : "";
    func = func ? func : "";
    expr = expr ? expr : "";
    fmt  = fmt  ? fmt  : "";

    if (warn_prehook) {
        va_list vl2;
        __va_copy(vl2, vl);
        warn_prehook(file, func, line, expr, priority, fmt, vl2);
        va_end(vl2);
    }

    if (likely((siz = vsnprintf(NULL, 0, fmt, vl)) >= 0)) {
        char *buf = (char *)alloca((size_t)siz);
        vsprintf(buf, fmt, vl);
        msg = buf;
    } else {
        msg = "No msg...vsnprintf() error inside reporting library";
    }

    list_for_each_entry(pos, &erd_list, list) {
        nu_wdt_clear();
        if (pos->min_priority <= priority)
            pos->op->report(pos, file, func, line, expr, priority, msg);
    }

    if (warn_posthook)
        warn_posthook(file, func, line, expr, priority, msg);

    warn_exit();
}

COLD void PRINTF(5,0)
nu_vpanic(char const * file, char const * func, const u32 line,
    char const * expr, char const * fmt, va_list vl)
{
    const char *msg = NULL;
    s32 siz;
    struct nu_error_reporting_dev *pos = NULL;

    if (!panic_enter())
        return;

    file = file ? file : "";
    func = func ? func : "";
    expr = expr ? expr : "";
    fmt  = fmt  ? fmt  : "";

    if (panic_prehook) {
        va_list vl2;
        __va_copy(vl2, vl);
        panic_prehook(file, func, line, expr, NU_REP_PANIC, fmt, vl2);
        va_end(vl2);
    }

    if (likely((siz = vsnprintf(NULL, 0, fmt, vl)) >= 0)) {
        char *buf = (char *)alloca((size_t)siz);
        vsprintf(buf, fmt, vl);
        msg = buf;
    } else {
        msg = "No msg...vsnprintf() error inside reporting library";
    }

    list_for_each_entry(pos, &erd_list, list) {
        nu_wdt_clear();
        pos->op->report(pos, file, func, line, expr, NU_REP_PANIC, msg);
    }

    if (panic_posthook)
        panic_posthook(file, func, line, expr, NU_REP_PANIC, msg);

    while (1)
        NU_NOP();

    unreachable();
}

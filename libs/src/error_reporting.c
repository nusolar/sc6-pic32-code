#include <stdarg.h>
#include <stdio.h>
#include "error_reporting.h"
#include "errorcodes.h"
#include "utility.h"
#include "wdt.h"

static struct error_reporting_dev *rep_devices[MAX_REPORTING_DEVS] = {NULL};

#define dev_is_empty(i) (NULL == rep_devices[(i)])
#define clear_dev(i)    rep_devices[(i)] = NULL

static s32 MUST_CHECK
get_free_err_dev_index(void)
{
    u16 ui;
    for (ui = 0; ui < ARRAY_SIZE(rep_devices); ++ui)
        if (dev_is_empty(ui))
            return ui;
    
    return -EREPORTNOFREEDEVS;
}

s32
register_reporting_dev(struct error_reporting_dev *erd,
                        enum report_priority min_priority)
{
    s32 tmp = get_free_err_dev_index();
    if (tmp < 0)
        return tmp;

    erd->min_priority = min_priority;
    rep_devices[tmp] = erd;

    return 0;
}

COLD void
unregister_reporting_dev(const struct error_reporting_dev *erd)
{
    u16 ui;
    for (ui = 0; ui < ARRAY_SIZE(rep_devices); ++ui)
        if (rep_devices[ui] == erd)
            clear_dev(ui);
}

/*
 * expr may be NULL if there is no expression to report
 */
static INLINE void PRINTF(7, 0)
vreportf (const char *file, const char *func, u32 line, const char *expr,
    enum report_priority priority, s32 err, const char *fmt, va_list arg)
{
    /* prevent recursion (also breaks thread-safeness, unfortunately) */
    static volatile s32 recurse_lock = 0;
    u32                 ui;
    char                formatted_msg[1024];
    const char          *err_str = get_error_name(err);
    const char          *blank = "";

    if (!file)
        file = blank;
    if (!func)
        func = blank;
    if (!expr)
        expr = blank;
    if (!fmt)
        fmt = blank;

    vsnprintf(formatted_msg, sizeof(formatted_msg), fmt, arg);

    if (recurse_lock)
        return;
    recurse_lock = 1;

    for (ui = 0; ui < ARRAY_SIZE(rep_devices); ++ui) {
        clear_wdt();
        if (dev_is_empty(ui) || rep_devices[ui]->min_priority > priority ||
                rep_devices[ui]->op->report == NULL)
            continue;

        rep_devices[ui]->op->report(rep_devices[ui], file, func, line, expr,
            priority, err, err_str, formatted_msg);
    }

    recurse_lock = 0;
}

void
err_clear(enum report_priority max_priority)
{
    u32 ui;
    for (ui = 0; ui < ARRAY_SIZE(rep_devices); ++ui) {
        clear_wdt();
        if (dev_is_empty(ui) || rep_devices[ui]->min_priority > max_priority ||
                rep_devices[ui]->op->reset_err_state == NULL)
            continue;
        
        rep_devices[ui]->op->reset_err_state(rep_devices[ui]);
    }
}

COLD void
reportf(const char *file, const char *func, u32 line, const char *expr,
    enum report_priority priority, s32 err, const char *fmt, ...)
{
    va_list fmtargs;
    va_start(fmtargs, fmt);
    vreportf(file, func, line, expr, priority, err, fmt, fmtargs);
    va_end(fmtargs);
}

#ifndef NU_ERROR_REPORTING_H
#define NU_ERROR_REPORTING_H

#include <stdarg.h>
#include "compiler.h"
#include "nu_types.h"
#include "utility.h"

#ifndef MAX_REPORTING_DEVS
# define MAX_REPORTING_DEVS  10
#endif

#define REPORTF(expr, priority, err_num, fmt...)    \
    reportf(__FILE__, __FUNCTION__, __LINE__, (expr), (priority), (err_num), ## fmt)

#define REPORT_ERR(priority, err_num, fmt...)   \
    REPORTF(NULL, priority, err_num, ## fmt)

#define REPORT(priority, fmt...)    \
    REPORTF(NULL, priority, ENONE, ## fmt)

#define IF_ERR(expr, priority, fmt...)                  \
    if (unlikely({                                      \
        s32 _errno = (s32)(expr);                       \
        if (unlikely(_errno < 0))                       \
            REPORTF(#expr, (priority), _errno, ## fmt); \
        _errno < 0;                                     \
    }))

#define IF_NOERR(expr, priority, fmt...)                \
    if (likely({                                        \
        s32 _errno = (s32)(expr);                       \
        if (unlikely(_errno < 0))                       \
            REPORTF(#expr, (priority), _errno, ## fmt); \
        _errno >= 0;                                    \
    }))

#define REPORT_ON_ERR(expr, priority, fmt...)           \
    do {                                                \
        s32 _errno = (s32)(expr);                       \
        if (_errno < 0)                                 \
            REPORTF(#expr, (priority), _errno, ## fmt); \
    } while(0)

enum report_priority {
    REP_DEBUG,
    REP_INFO,
    REP_NOTICE,
    REP_WARNING,
    REP_ERROR,
    REP_CRITICAL,
    REP_ALERT,
    REP_EMERGENCY
};

struct error_reporting_dev {
    const struct vtbl_error_reporting_dev *op;
    enum report_priority min_priority;
};

struct vtbl_error_reporting_dev {
    s32 (*report)          (struct error_reporting_dev *self,
                                const char *file, const char *func, u32 line,
                                const char *expr,
                                enum report_priority priority,
                                s32 errNum, const char *errName,
                                const char *fmtdMsg);
    s32 (*reset_err_state) (struct error_reporting_dev *self);
};

#define ERD_INIT(priority, _op)  \
    {.op = _op, .min_priority = priority}

static ALWAYSINLINE void
INIT_ERD(struct error_reporting_dev *e, enum report_priority min_prior,
        const struct vtbl_error_reporting_dev *op)
{
    e->min_priority = min_prior;
    e->op = op;
}

s32
register_reporting_dev(struct error_reporting_dev *erd,
                        enum report_priority min_priority);

void
unregister_reporting_dev(const struct error_reporting_dev *erd);

void
err_clear(enum report_priority max_priority);

COLD void PRINTF(7, 8)
reportf(const char *file, const char *func, u32 line, const char *expr,
        enum report_priority priority, s32 err, const char *fmt, ...);

#endif

#ifndef NU_ERROR_REPORTING_H
#define NU_ERROR_REPORTING_H 1

#include "boost/preprocessor.hpp"
#include "nu/compiler.h"
#include "nu/list.h"
#include "nu/nu_types.h"
#include <stdarg.h>

/* if we have variadic macros... */
#if defined BOOST_PP_VARIADICS && BOOST_PP_VARIADICS

/* example invocation:
 * NU_PANIC("Battery %d has temp %fºC", ui, temps[ui]);
 */
# define NU_PANIC(fmt, ...) \
    nu_panic(__FILE__, __FUNCTION__, __LINE__, NULL, /*"PANIC:"*/ fmt, __VA_ARGS__)

/* example invocation:
 * NU_PANIC_ON(temps[ui] > MAX_TEMP, "Battery %d has temp %fºC!", ui, temps[ui]);
 */
# define NU_PANIC_ON(cond, fmt, ...) \
    nu_panic_on(!!(cond), __FILE__, __FUNCTION__, __LINE__, #cond, /*"PANIC:"*/ fmt, __VA_ARGS__)

/* example invocation:
 * NU_WARN(NU_REP_DEBUG, "Debug: temp %d = %fºC.", ui, temps[ui]);
 */
# define NU_WARN(priority, fmt, ...) \
    nu_warn(__FILE__, __FUNCTION__, __LINE__, NULL, /*"WARN:"*/ fmt, __VA_ARGS__)

/* example invocation:
 * if (NU_WARN_ON(ui > 10, NU_REP_EMERGENCY, "Max ui value exceeded! ui is %u\n", ui)) {
 *     try_again(ui);
 * }
 */
# define NU_WARN_ON(cond, priority, fmt, ...) \
    nu_warn_on(!!(cond), __FILE__, __FUNCTION__, __LINE__, #cond, priority, /*"WARN:"*/ fmt, __VA_ARGS__)

#else   /* no variadic macros */

/* example invocation:
 * NU_PANIC(("Panic: battery %d has temp %fºC") (ui) (temps[ui]));
 */
# define NU_PANIC(seq /* (fmt) ... */) \
    nu_panic(__FILE__, __FUNCTION__, __LINE__, NULL, /*"PANIC:"*/ BOOST_PP_SEQ_ENUM(seq))

/* example invocation:
 * NU_PANIC_ON((temps[ui] > MAX_TEMP) ("Battery %d has temp %fºC!") (ui) (temps[ui]));
 */
# define NU_PANIC_ON(seq /* (cond) (fmt) ... */) \
    nu_panic_on(!!(BOOST_PP_SEQ_HEAD(seq)), __FILE__, __FUNCTION__, __LINE__, BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(seq)), /*"PANIC:"*/ BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq)))

/* example invocation:
 * NU_WARN((NU_REP_DEBUG) ("Debug: temp %d = %fºC.") (ui) (temps[ui]));
 */
# define NU_WARN(seq /* (priority) (fmt) ... */) \
    nu_warn(__FILE__, __FUNCTION__, __LINE__, NULL, BOOST_PP_SEQ_HEAD(seq), /*"WARN:"*/ BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq)))

/* example invocation:
 * if (NU_WARN_ON((ui > 10) (NU_REP_EMERGENCY) ("Max ui value exceeded! ui is %u\n") (ui))) {
 *     try_again(ui);
 * }
 */
# define NU_WARN_ON(seq /* (cond) (priority) (fmt) ... */) \
    nu_warn_on(!!(BOOST_PP_SEQ_HEAD(seq)), __FILE__, __FUNCTION__, __LINE__, BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(seq)), BOOST_PP_SEQ_ELEM(1, seq), /*"WARN:"*/ BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(BOOST_PP_SEQ_TAIL(seq))))

#endif

#define NU_REPORT_PRIORITIES(_) \
    _(IGNORE) \
    _(DEBUG) \
    _(INFO) \
    _(NOTICE) \
    _(WARNING) \
    _(ERROR) \
    _(CRITICAL) \
    _(EMERGENCY) \
    _(PANIC)

enum nu_report_priority {
#define NU_PRIORITY_AS_ENUM(x) NU_REP_##x,
    NU_REPORT_PRIORITIES(NU_PRIORITY_AS_ENUM)
    NU_NUM_REPORT_PRIORITIES
};

extern const char * const nu_report_priority_str[];

extern bool nu_library_warn_enabled;

struct nu_error_reporting_dev {
    struct list_head list;
    const struct nu_vtbl_error_reporting_dev *op;
    enum nu_report_priority min_priority;
};
#define NU_ERD_INIT(priority, op)  \
    {{NULL, NULL}, (op), (priority)}

struct nu_vtbl_error_reporting_dev {
    void (*report)          (struct nu_error_reporting_dev *erd,
                                const char *file, const char *func, u32 line,
                                const char *expr,
                                enum nu_report_priority priority,
                                const char *fmtd_msg);
    void (*reset_err_state) (struct nu_error_reporting_dev *erd);
};

static ALWAYSINLINE void
NU_INIT_ERD(struct nu_error_reporting_dev *e, enum nu_report_priority min_prior,
        const struct nu_vtbl_error_reporting_dev *op)
{
    INIT_LIST_HEAD(&(e->list));
    e->min_priority = min_prior;
    e->op = op;
}

/* this allows you to, for example, disable clearing of WDT before doing any
 * actual reporting (potentially useful for implementing tripping).
 * Hooks (pre- and post-) also allow for easy implementation
 * of other custom error-reporting behavior without having to resort to a
 * full-blown error reporting device (ERD).
 */
typedef void (*nu_reporting_prehook)
(const char *file, const char *func, u32 line, const char *expr,
        enum nu_report_priority pri, const char *fmt, va_list vl);

typedef void (*nu_reporting_posthook)
(const char *file, const char *func, u32 line, const char *expr,
        enum nu_report_priority pri, const char *fmtd_msg);

COLD void
nu_reporting_register_warn_prehook(nu_reporting_prehook func);

COLD void
nu_reporting_register_warn_posthook(nu_reporting_posthook func);

COLD void
nu_reporting_register_panic_prehook(nu_reporting_prehook func);

COLD void
nu_reporting_register_panic_posthook(nu_reporting_posthook func);

COLD void PRINTF(5,0)
nu_vpanic(const char *file, const char *func, u32 line, const char *expr,
        const char *fmt, va_list args);

#ifdef IS_NU_LIBRARY
COMPILE_ERROR(("Can't invoke panic from library code!"))
#endif
static COLD NORETURN void PRINTF(5,6)
nu_panic(const char *file, const char *func, u32 line, const char *expr,
    const char *fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    nu_vpanic(file, func, line, expr, fmt, vl);
    unreachable();
}

#ifdef IS_NU_LIBRARY
COMPILE_ERROR(("Can't invoke panic from library code!"))
#endif
static COLD void PRINTF(6,7)
nu_panic_on(u32 cond, const char *file, const char *func, u32 line,
        const char *expr, const char *fmt, ...)
{
    if (unlikely((long)cond)) {
        va_list vl;
        va_start(vl, fmt);
        nu_vpanic(file, func, line, expr, fmt, vl);
        unreachable();
    }
}

COLD void PRINTF(6,0)
nu_vwarn(const char *file, const char *func, u32 line, const char *expr,
    enum nu_report_priority priority, const char *fmt, va_list args);

static COLD void PRINTF(6, 7)
nu_warn(const char *file, const char *func, u32 line, const char *expr,
        enum nu_report_priority priority, const char *fmt, ...)
{
    va_list vl;
#ifdef IS_NU_LIBRARY
    if (!nu_library_warn_enabled)
        return;
#endif
    va_start(vl, fmt);
    nu_vwarn(file, func, line, expr, priority, fmt, vl);
    va_end(vl);
}

static COLD u32 PRINTF(7, 8)
nu_warn_on(u32 cond, const char *file, const char *func, u32 line,
    const char *expr, enum nu_report_priority priority, const char *fmt, ...)
{
#ifdef IS_NU_LIBRARY
    if (!nu_library_warn_enabled)
        return cond;
#endif
    if (unlikely((long)cond)) {
        va_list vl;
        va_start(vl, fmt);
        nu_vwarn(file, func, line, expr, priority, fmt, vl);
        va_end(vl);
    }
    return cond;
}

void
nu_reporting_register(struct nu_error_reporting_dev *erd);

void
nu_reporting_unregister(struct nu_error_reporting_dev *erd);

void
nu_reporting_clear(enum nu_report_priority max_priority);

#endif

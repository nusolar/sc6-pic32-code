#ifndef NU_ERROR_REPORTING_H
#define NU_ERROR_REPORTING_H 1

#include "compiler.h"
#include "list.h"
#include "nu_types.h"
#include "utility.h"
#include "boost/preprocessor.hpp"

#include <stdarg.h>

#if defined BOOST_PP_VARIADICS && BOOST_PP_VARIADICS

# define NU_PANIC(fmt, ...) \
    nu_panic(__FILE__, __FUNCTION__, __LINE__, NULL, fmt, __VA_ARGS__)
# define NU_PANIC_ON(cond, fmt, ...) \
    (_nu_ret_warn_on = !!(cond), \
    unlikely(_ret_warn_on) ? \
        panic(__FILE__, __FUNCTION__, __LINE__, #cond, fmt, __VA_ARGS__), \
        _nu_ret_warn_on \
        : 0)

/* example invocation:
 * NU_WARN_ON(ui > 10, NU_REP_EMERGENCY, "Max ui value exceeded! ui is %u\n", ui);
 */

# define NU_WARN(priority, fmt, ...) \
    nu_warn(__FILE__, __FUNCTION__, __LINE__, NULL, fmt, __VA_ARGS__)
# define NU_WARN_NULL(priority) \
    WARN(priority, NULL)
# define NU_WARN_ON(cond, priority, fmt, ...) \
    (_nu_ret_warn_on = !!(cond), \
    unlikely(_nu_ret_warn_on) ? \
        nu_warn(__FILE__, __FUNCTION__, __LINE__, #cond, priority, fmt, __VA_ARGS__), \
            _nu_ret_warn_on \
        : 0)

# define NU_WARN_DEBUG(fmt, ...)           NU_WARN(NU_REP_DEBUG, fmt, __VA_ARGS__)
# define NU_WARN_DEBUG_NULL()              NU_WARN_NULL(NU_REP_DEBUG)
# define NU_WARN_DEBUG_ON(cond, fmt, ...)  NU_WARN_ON(cond, NU_REP_DEBUG, fmt, __VA_ARGS__)

# define NU_WARN_INFO(fmt, ...)            NU_WARN(NU_REP_INFO, fmt, __VA_ARGS__)
# define NU_WARN_INFO_NULL()               NU_WARN_NULL(NU_REP_INFO)
# define NU_WARN_INFO_ON(cond, fmt, ...)   NU_WARN_ON(cond, NU_REP_INFO, fmt, __VA_ARGS__)

# define NU_WARN_NOTICE(fmt, ...)          NU_WARN(NU_REP_NOTICE, fmt, __VA_ARGS__)
# define NU_WARN_NOTICE_NULL()             NU_WARN_NULL(NU_REP_NOTICE)
# define NU_WARN_NOTICE_ON(cond, fmt, ...) NU_WARN_ON(cond, NU_REP_NOTICE, fmt, __VA_ARGS__)

#else   /* no variadic macros */

# define NU_PANIC(seq /* (priority) (fmt) ... */)    \
    nu_panic(__FILE__, __FUNCTION__, __LINE__, NULL, BOOST_PP_SEQ_ENUM(seq))
# define NU_PANIC_ON(seq /* (cond) (priority) (fmt) ... */)   \
    do { \
        if (unlikely(!!(BOOST_PP_SEQ_HEAD(seq)))) \
            nu_panic(__FILE__, __FUNCTION__, __LINE__, BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD), BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq))); \
    } while(0)

/* example invocation:
 * NU_WARN_ON((ui > 10) (NU_REP_EMERGENCY) ("Max ui value exceeded! ui is %u\n") (ui));
 */

# define NU_WARN(seq /* (priority) (fmt) ... */) \
    warn(__FILE__, __FUNCTION__, __LINE__, NULL, BOOST_PP_SEQ_ENUM(seq))
# define NU_WARN_NULL(priority) WARN((priority) (NULL))
# define NU_WARN_ON(seq /* (cond) (priority) (fmt) ... */) \
    (_nu_ret_warn_on = !!(BOOST_PP_SEQ_HEAD(seq)), \
    unlikely(_nu_ret_warn_on) ? \
        nu_warn(__FILE__, __FUNCTION__, __LINE__, BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(seq)), BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq))), \
            _nu_ret_warn_on \
        : 0)

# define NU_WARN_DEBUG(seq)    NU_WARN(BOOST_PP_SEQ_PUSH_FRONT(NU_REP_DEBUG, seq))
# define NU_WARN_DEBUG_NULL()  NU_WARN_NULL(NU_REP_DEBUG)
# define NU_WARN_DEBUG_ON(seq) NU_WARN_ON(BOOST_PP_SEQ_HEAD(seq), NU_REP_DEBUG, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq)))

# define NU_WARN_INFO(seq)     NU_WARN(BOOST_PP_SEQ_PUSH_FRONT(NU_REP_INFO, seq))
# define NU_WARN_INFO_NULL()   NU_WARN_NULL(NU_REP_INFO)
# define NU_WARN_INFO_ON(seq)  NU_WARN_ON(BOOST_PP_SEQ_HEAD(seq), NU_REP_INFO, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq)))

# define NU_WARN_NOTICE(seq)       NU_WARN(BOOST_PP_SEQ_PUSH_FRONT(NU_REP_NOTICE, seq))
# define NU_WARN_NOTICE_NULL()     NU_WARN_NULL(NU_REP_NOTICE)
# define NU_WARN_NOTICE_ON(seq)    NU_WARN_ON(BOOST_PP_SEQ_HEAD(seq), NU_REP_NOTICE, BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(seq)))

#endif

enum nu_report_priority {
    NU_REP_PRIORITY_0,
    NU_REP_DEBUG        = NU_REP_PRIORITY_0,
    NU_REP_PRIORITY_1,
    NU_REP_INFO         = NU_REP_PRIORITY_1,
    NU_REP_PRIORITY_2,
    NU_REP_NOTICE       = NU_REP_PRIORITY_2,
    NU_REP_PRIORITY_3,
    NU_REP_WARNING      = NU_REP_PRIORITY_3,
    NU_REP_PRIORITY_4,
    NU_REP_ERROR        = NU_REP_PRIORITY_4,
    NU_REP_PRIORITY_5,
    NU_REP_CRITICAL     = NU_REP_PRIORITY_5,
    NU_REP_PRIORITY_6,
    NU_REP_EMERGENCY    = NU_REP_PRIORITY_6,
    NU_REP_PANIC        = NU_REP_EMERGENCY
};

typedef void (*nu_reporting_prehook)
(const char *file, const char *func, u32 line, const char *expr,
        enum nu_report_priority pri, const char *fmt, va_list ap);

typedef void (*nu_reporting_posthook)
(const char *file, const char *func, u32 line, const char *expr,
        enum nu_report_priority pri, const char *fmtd_msg);

struct nu_error_reporting_dev {
    struct list_head list;
    const struct nu_vtbl_error_reporting_dev *op;
    enum nu_report_priority min_priority;
};
#define NU_ERD_INIT(priority, op)  \
    {{NULL, NULL}, (op), (priority)}

struct nu_vtbl_error_reporting_dev {
    s32 (*report)          (struct nu_error_reporting_dev *self,
                                const char *file, const char *func, u32 line,
                                const char *expr,
                                enum nu_report_priority priority,
                                const char *msg);
    s32 (*reset_err_state) (struct nu_error_reporting_dev *self);
};

#ifdef __cplusplus
extern "C" {
#endif

COLD void
nu_reporting_register_warn_prehook(nu_reporting_prehook func);

COLD void
nu_reporting_register_warn_posthook(nu_reporting_posthook func);

COLD void PRINTF(6,7)
nu_warn(const char *file, const char *func, u32 line, const char *expr,
    enum nu_report_priority priority, const char *fmt, ...);

COLD void
nu_reporting_register_panic_prehook(nu_reporting_prehook func);

COLD void
nu_reporting_register_panic_posthook(nu_reporting_posthook func);

COLD NORETURN void PRINTF(5,6)
nu_panic(const char *file, const char *func, u32 line, const char *expr,
    const char *fmt, ...);

static ALWAYSINLINE void
NU_INIT_ERD(struct nu_error_reporting_dev *e, enum nu_report_priority min_prior,
        const struct nu_vtbl_error_reporting_dev *op)
{
    INIT_LIST_HEAD(&(e->list));
    e->min_priority = min_prior;
    e->op = op;
}

void
nu_reporting_register(struct nu_error_reporting_dev *erd);

void
nu_reporting_unregister(struct nu_error_reporting_dev *erd);

void
nu_reporting_clear(enum nu_report_priority max_priority);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif

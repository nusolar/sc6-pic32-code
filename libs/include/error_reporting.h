#ifndef __NU_ERROR_REPORTING_H
#define __NU_ERROR_REPORTING_H

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "errorcodes.h"
#include "safestring.h"
#include "utility.h"

#ifndef MAX_REPORTING_DEVS
    #define MAX_REPORTING_DEVS  10
#endif

/* REPORTF((priority, errNum, expr, fmt, ...)) */
#define REPORTF(x)  do {                                                \
                        setErrBuf(__FILE__, __LINE__, 0, NULL);         \
                        reportf_fileLineBuf x;                          \
                    } while(0)

#define REPORT_ERR_EXPR(x) REPORTF(x)

/* REPORT_ERR((priority, errNum, fmt, ...)) */
#define REPORT_ERR(x) \
                    do {                                                \
                        setErrBuf(__FILE__, __LINE__, 0, NULL);         \
                        reportf_fileLineExprBuf x;                      \
                    } while(0)

/* REPORT((report_pri, fmt, ...)) */
#define REPORT(x)                                                           \
                    do {                                                    \
                        setErrBuf(__FILE__, __LINE__, ENONE, NULL);         \
                        reportf_fileLineExprErrBuf x;                       \
                    } while(0)

extern int32_t nu_errno;

#define IF_ERR(expr, rep_pri, err_msg)                                      \
            if ((nu_errno = (int32_t)(expr)) < 0 &&                         \
                setErrBuf(__FILE__, __LINE__, 0, NULL),                     \
                (reportf_fileLineBuf(rep_pri, nu_errno, #expr, err_msg) || 1))

#define IF_NOERR(expr, rep_pri, err_msg)                                    \
            if (!((nu_errno = (int32_t)(expr)) < 0 &&                       \
                setErrBuf(__FILE__, __LINE__, 0, NULL),                     \
                (reportf_fileLineBuf(rep_pri, nu_errno, #expr, err_msg) || 1)))

#define REPORT_ON_ERR(expr, rep_pri, err_msg)                               \
            do {                                                            \
                nu_errno = (int32_t)(expr);                                 \
                if (nu_errno < 0)                                           \
                    REPORT_ERR_EXPR((rep_pri, nu_errno, #expr, err_msg));   \
            } while(0)

enum report_priority {
    REP_DEBUG,
    REP_INFO,
    REP_NOTICE,
    REP_WARNING,
    REP_ERROR,
    REP_CRITICAL,
    REP_ALERT,
    REP_EMERGENCY,
};

struct error_reporting_dev {
    const struct vtblError_reporting_dev *op;
    enum report_priority minPriority;
};

struct vtblError_reporting_dev {
    int32_t (*report)          (struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg);
    int32_t (*resetErrState)   (struct error_reporting_dev *self);
};

int32_t
register_reporting_dev(struct error_reporting_dev *erd,
                        enum report_priority minPriority);

int32_t
unregister_reporting_dev(const struct error_reporting_dev *erd);

int32_t
printErrInfo(char *dst, size_t dstSiz, enum report_priority priority,
                int32_t errNum, const char *errName);

int32_t
printContextInfo(char *dst, size_t dstSiz, const char *expr, const char *file,
                    uint32_t line);

void
setErrBuf(const char *file, uint32_t line, int32_t err, const char *expr);

void
err_clear(enum report_priority maxPriority);

void
reportf(const char *file, uint32_t line, enum report_priority priority,
        int32_t errNum, const char *expr, const char *fmt, ...) __attribute__((format(printf,6,7)));

void
reportf_fileLineBuf(enum report_priority priority, int32_t errNum,
        const char *expr, const char *fmt, ...) __attribute__((format(printf,4,5)));

void
reportf_fileLineExprBuf(enum report_priority priority, int32_t errNum,
        const char *fmt, ...) __attribute__((format(printf,3,4)));

void
reportf_fileLineExprErrBuf(enum report_priority priority,
        const char *fmt, ...) __attribute__((format(printf,2,3)));

#endif

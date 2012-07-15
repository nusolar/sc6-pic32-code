#include "../include/error_reporting.h"

static struct error_reporting_dev *repDevices[MAX_REPORTING_DEVS] = {NULL};

inline static int32_t
devIsEmpty(uint32_t i)
{
    return (repDevices[i] == NULL);
}

inline static void
clearDev(uint32_t i)
{
    repDevices[i] = NULL;
}

static int32_t
getFreeErrDevIndex(void)
{
    uint16_t ui;
    for (ui = 0; ui < ARRAY_SIZE(repDevices); ui++) {
        ClearWDT();
        if (devIsEmpty(ui))
            return ui;
    }
    
    return -EREPORTNOFREEDEVS;
}

int32_t
register_reporting_dev(struct error_reporting_dev *erd,
                        enum report_priority minPriority)
{
    int32_t tmp;

    if (!erd)
        return -ENULPTR;

    tmp = getFreeErrDevIndex();
    if (tmp < 0)
        return tmp;

    erd->minPriority = minPriority;
    repDevices[tmp] = erd;

    return 0;
}

int32_t
unregister_reporting_dev(const struct error_reporting_dev *erd)
{
    uint32_t ui;

    if (erd == NULL)
        return -ENULPTR;

    for (ui = 0; ui < ARRAY_SIZE(repDevices); ui++) {
        CLEARWDT();
        if (repDevices[ui] == erd)
            clearDev(ui);
    }

    return 0;
}

int32_t
printErrInfo(char *dst, size_t dstSiz, enum report_priority priority,
                int32_t errNum, const char *errName)
{
    if (dst == NULL || errName == NULL)
        return -ENULPTR;

    snprintf(dst, dstSiz, "ERR<%d>%d(%s)", priority, errNum, errName);
    return 0;
}

int32_t
printContextInfo(char *dst, size_t dstSiz, const char *expr, const char *file,
                    uint32_t line)
{
    if (dst == NULL || expr == NULL || file == NULL)
        return -ENULPTR;

    snprintf(dst, sizeof(dstSiz), "%s\n\t@%s:%d", expr, file, line);
    return 0;
}

/*
 * expr may be NULL if there is no expression to report
 */
static void
__attribute__ ((format (printf, 6, 0)))
vreportf (const char *file, uint32_t line, enum report_priority priority,
        int32_t errNum, const char *expr, const char *fmt, va_list arg)
{
    /* prevent recursion (also breaks thread-safeness, unfortunately) */
    static int32_t      vreportfRecurseLock = 0;
    uint32_t            ui;
    char                formattedMsg[1024];
    const char          *errName = getErrorName(errNum);
    const char          *blank = "";

    if (!file || !fmt)
        return;

    if (!expr)
        expr = blank;

    vsnprintf(formattedMsg, sizeof(formattedMsg), fmt, arg);   

    if (vreportfRecurseLock)
        return;
    vreportfRecurseLock = 1;

    for (ui = 0; ui < ARRAY_SIZE(repDevices); ui++) {
        CLEARWDT();
        if (devIsEmpty(ui) || repDevices[ui]->minPriority > priority ||
                repDevices[ui]->op->report == NULL)
            continue;

        repDevices[ui]->op->report(repDevices[ui], file, line, expr, priority,
                                    errNum, errName, formattedMsg);
    }

    vreportfRecurseLock = 0;
}

void
err_clear(enum report_priority maxPriority)
{
    uint32_t ui;
    for (ui = 0; ui < ARRAY_SIZE(repDevices); ui++) {
        CLEARWDT();
        if (devIsEmpty(ui) || repDevices[ui]->minPriority > maxPriority ||
                repDevices[ui]->op->resetErrState == NULL)
            continue;
        
        repDevices[ui]->op->resetErrState(repDevices[ui]);
    }
}

void
reportf(const char *file, uint32_t line, enum report_priority priority,
        int32_t errNum, const char *expr, const char *fmt, ...)
{
    va_list fmtargs;
    va_start(fmtargs, fmt);
    vreportf(file, line, priority, errNum, expr, fmt, fmtargs);
    va_end(fmtargs);
}

static char     filebuf[1000] = {};
static char     exprbuf[1000] = {};
static uint32_t linebuf = 0;
static int32_t  errbuf = 0;

void
setErrBuf(const char *file, uint32_t line, int32_t err, const char *expr)
{
    linebuf = line;
    errbuf  = err;

    if (!file)
        filebuf[0] = '\0';
    else
        strlcpy(filebuf, file, ARRAY_SIZE(filebuf));

    if (!expr)
        exprbuf[0] = '\0';
    else
        strlcpy(exprbuf, expr, ARRAY_SIZE(exprbuf));
}

void
reportf_fileLineBuf(enum report_priority priority, int32_t errNum,
        const char *expr, const char *fmt, ...)
{
    va_list fmtargs;
    va_start(fmtargs, fmt);
    vreportf(filebuf, linebuf, priority, errNum, expr, fmt, fmtargs);
    va_end(fmtargs);
}

void
reportf_fileLineExprBuf(enum report_priority priority, int32_t errNum,
        const char *fmt, ...)
{
    va_list fmtargs;
    va_start(fmtargs, fmt);
    vreportf(filebuf, linebuf, priority, errNum, exprbuf, fmt, fmtargs);
    va_end(fmtargs);
}

void
reportf_fileLineExprErrBuf(enum report_priority priority, const char *fmt, ...)
{
    va_list fmtargs;
    va_start(fmtargs, fmt);
    vreportf(filebuf, linebuf, priority, errbuf, exprbuf, fmt, fmtargs);
    va_end(fmtargs);
}

#include "nu/component/led.h"

static void
nu__Led__reset_err_state(struct nu__ErrorReportingDev *erd)
{
    nu__Led__off(nu_erd_to_led(erd));
}

static void
nu__Led__report(struct nu__ErrorReportingDev *e,
            UNUSED const char *file, UNUSED const char *func, UNUSED u32 line,
            UNUSED const char *expr,
            UNUSED enum nu_report_priority priority,
            UNUSED const char *fmtd_msg)
{
    nu__Led__on(nu_erd_to_led(e));
}

const struct nu__ErrorReportingDev__Vtbl nu__Led__erd_ops = {
    nu__Led__report,
    nu__Led__reset_err_state,
};

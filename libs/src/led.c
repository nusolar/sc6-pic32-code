#include "led.h"

static s32
led_resetErrState(struct nu_error_reporting_dev *erd)
{
    nu_led_off(erd_to_led(erd));
    return 0;
}

static s32
led_report(struct nu_error_reporting_dev *erd,
            UNUSED const char *file, UNUSED const char *func, UNUSED u32 line,
            UNUSED const char *expr, UNUSED enum report_priority priority,
            UNUSED s32 err, UNUSED const char *err_str,
            UNUSED const char *fmtd_msg)
{
    nu_led_on(erd_to_led(erd));
    return 0;
}

const struct nu_vtbl_error_reporting_dev led_erd_ops = {
    .report         = &led_report,
    .reset_err_state= &led_resetErrState,
};

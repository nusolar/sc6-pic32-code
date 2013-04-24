#include "nu/led.h"

static s32
led_resetErrState(struct nu_error_reporting_dev *erd)
{
    nu_led_off(nu_erd_to_led(erd));
    return 0;
}

static s32
led_report(struct nu_error_reporting_dev *e,
            UNUSED const char *file, UNUSED const char *func, UNUSED u32 line,
            UNUSED const char *expr,
            UNUSED enum nu_report_priority priority,
            UNUSED const char *msg)
{
    nu_led_on(nu_erd_to_led(e));
    return 0;
}

const struct nu_vtbl_error_reporting_dev led_erd_ops = {
    &led_report,
    &led_resetErrState,
};

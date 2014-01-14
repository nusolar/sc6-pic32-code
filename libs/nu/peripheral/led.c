#include "nu/component/led.h"

static void
nu_led_reset_err_state(struct nu_error_reporting_dev *erd)
{
    nu_led_off(nu_erd_to_led(erd));
}

static void
nu_led_report(struct nu_error_reporting_dev *e,
            UNUSED const char *file, UNUSED const char *func, UNUSED u32 line,
            UNUSED const char *expr,
            UNUSED enum nu_report_priority priority,
            UNUSED const char *fmtd_msg)
{
    nu_led_on(nu_erd_to_led(e));
}

const struct nu_vtbl_error_reporting_dev nu_led_erd_ops = {
    nu_led_report,
    nu_led_reset_err_state,
};

#include "../include/led.h"

static int32_t
led_on(const struct led *self);

static int32_t
led_off(const struct led *self);

static int32_t
led_toggle(const struct led *self);

static const struct vtblLed ledOps = {
    .on     = &led_on,
    .off    = &led_off,
    .toggle = &led_toggle,
};

static int32_t
led_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg);

static int32_t
led_resetErrState(struct error_reporting_dev *self);

static const struct vtblError_reporting_dev led_erd_ops = {
    .report         = &led_report,
    .resetErrState  = &led_resetErrState,
};

static int32_t
led_report(struct error_reporting_dev *self,
                                const char *file, uint32_t line,
                                const char *expr,
                                enum report_priority priority,
                                int32_t errNum, const char *errName,
                                const char *fmtdMsg)
{
    struct led *ledSelf;

    (void)file;
    (void)line;
    (void)expr;
    (void)priority;
    (void)errNum;
    (void)errName;
    (void)fmtdMsg;

    if (self == NULL)
        return -ENULPTR;
    
    ledSelf = error_reporting_dev_to_led(self);
    return ledSelf->op->on(ledSelf);
}

static int32_t
led_resetErrState(struct error_reporting_dev *self)
{
    struct led *ledSelf;

    if (self == NULL)
        return -ENULPTR;

    ledSelf = error_reporting_dev_to_led(self);
    return ledSelf->op->off(ledSelf);
}

static int32_t
led_init(const struct led *self)
{
    if (self == NULL)
        return -ENULPTR;
        
    PORTSetPinsDigitalOut(self->ledPinLtr, self->ledPinNum);
    
    return self->op->off(self);
}

int32_t
led_new(struct led *self, IoPortId ledPinLtr, uint32_t ledPinNum)
{
    if (self == NULL)
        return -ENULPTR;
        
    self->op        = &ledOps;
    self->erd.op    = &led_erd_ops;
    
    self->ledPinLtr = ledPinLtr;
    self->ledPinNum = ledPinNum;
    
    return led_init(self);
}

static int32_t
led_on(const struct led *self)
{
    if (self == NULL)
        return -ENULPTR;
    
    PORTClearBits(self->ledPinLtr, self->ledPinNum);
    
    return 0;
}

static int32_t
led_off(const struct led *self)
{
    if (self == NULL)
        return -ENULPTR;
    
    PORTSetBits(self->ledPinLtr, self->ledPinNum);
    
    return 0;
}

static int32_t
led_toggle(const struct led *self)
{
    if (self == NULL)
        return -ENULPTR;
    
    PORTToggleBits(self->ledPinLtr, self->ledPinNum);
    
    return 0;
}

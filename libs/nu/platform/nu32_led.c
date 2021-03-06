#include "nu/compiler.h"
#include "nu/platform/nu32_led.h"

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32.c"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

static struct nu__Led _nu32_led0;
static struct nu__Led _nu32_led1;
static struct nu__Led _nu32_led2;

struct nu__Led *nu__Nu32__led0 = &_nu32_led0;
struct nu__Led *nu__Nu32__led1 = &_nu32_led1;
struct nu__Led *nu__Nu32__led2 = &_nu32_led2;

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static ALWAYSINLINE void
nu__Nu32__setup_led_nu32_v1(void)
{
    struct nu__Led led1 =
        NU_LED_INIT(NU_LED_ERD_INIT(NU_REP_DEBUG), NU_PIN_INIT(NU_PIN_A4));
    struct nu__Led led2 =
        NU_LED_INIT(NU_LED_ERD_INIT(NU_REP_DEBUG), NU_PIN_INIT(NU_PIN_A5));
    nu__Nu32__led0 = nu__Nu32__led2;
    _nu32_led1 = led1;
    _nu32_led2 = led2;
}

static ALWAYSINLINE void
nu__Nu32__setup_led_nu32_v2(void)
{
    struct nu__Led led0 =
        NU_LED_INIT(NU_LED_ERD_INIT(NU_REP_DEBUG), NU_PIN_INIT(NU_PIN_G12));
    struct nu__Led led1 =
        NU_LED_INIT(NU_LED_ERD_INIT(NU_REP_DEBUG), NU_PIN_INIT(NU_PIN_G13));
    _nu32_led0 = led0;
    _nu32_led1 = led1;
    nu__Nu32__led2 = nu__Nu32__led0;
}

void
nu__Nu32__setup_led(nu__Nu32__Version_t version)
{
    switch (version) {
    case NU_NU32_V1:
        nu__Nu32__setup_led_nu32_v1();
        break;
    case NU_NU32_V2:
        nu__Nu32__setup_led_nu32_v2();
        break;
    default:
        break;
    }
    /* Configure LED outputs */
    nu__Led__setup(nu__Nu32__led1);
    nu__Led__setup(nu__Nu32__led2);
}

#pragma GCC diagnostic error "-Wdeprecated-declarations"

#endif /* NU_PLATFORM switch */

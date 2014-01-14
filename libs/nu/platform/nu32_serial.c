#include "nu/platform/nu32_serial.h"

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32.c"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

static struct nu__Serial _nu32_serial;
static struct nu__Serial _nu32_serial1;
static struct nu__Serial _nu32_serial2;

struct nu__Serial *nu__Nu32__serial = &_nu32_serial;
struct nu__Serial *nu__Nu32__serial1 = &_nu32_serial1;
struct nu__Serial *nu__Nu32__serial2 = &_nu32_serial2;

 #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static ALWAYSINLINE void
nu__Nu32__setup_serial_v1(void)
{
    nu_init_serial_platform_args_t args1 = NU_SERIAL_UART1;
    nu_init_serial_platform_args_t args2 = NU_SERIAL_UART4; /* used by bootloader */
    NU_INIT_SERIAL(nu__Nu32__serial, &args1);
    NU_INIT_SERIAL(nu__Nu32__serial2, &args2);
    nu__Nu32__serial1 = nu__Nu32__serial;
}

static ALWAYSINLINE void
nu__Nu32__setup_serial_v2(void)
{
    nu_init_serial_platform_args_t args1 = NU_SERIAL_UART1;
    nu_init_serial_platform_args_t args2 = NU_SERIAL_UART3;  /* = UART2A */
    NU_INIT_SERIAL(nu__Nu32__serial, &args1);
    NU_INIT_SERIAL(nu__Nu32__serial2, &args2);
    nu__Nu32__serial1 = nu__Nu32__serial;
}

 #pragma GCC diagnostic error "-Wdeprecated-declarations"

void
nu__Nu32__setup_serial(nu__Nu32__Version_t version)
{
    switch (version) {
    case NU_NU32_V1:
        nu__Nu32__setup_serial_v1();
        break;
    case NU_NU32_V2:
        nu__Nu32__setup_serial_v2();
        break;
    default:
        return;
    }
    nu__Serial__setup(nu__Nu32__serial1, 115200, &nu__Serial__platform_setup_defaults);
    nu__Serial__setup(nu__Nu32__serial2, 115200, &nu__Serial__platform_setup_defaults);
}

#endif /* NU_PLATFORM switch */

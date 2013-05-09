#include "nu/platform/nu32_serial.h"

static struct nu_serial _nu32_serial;
static struct nu_serial _nu32_serial1;
static struct nu_serial _nu32_serial2;

struct nu_serial *nu_nu32_serial = &_nu32_serial;
struct nu_serial *nu_nu32_serial1 = &_nu32_serial1;
struct nu_serial *nu_nu32_serial2 = &_nu32_serial2;

 #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static ALWAYSINLINE void
nu_nu32_setup_serial_v1(void)
{
    nu_init_serial_platform_args_t args1 = NU_SERIAL_UART1;
    nu_init_serial_platform_args_t args2 = NU_SERIAL_UART4; /* used by bootloader */
    NU_INIT_SERIAL(nu_nu32_serial, &args1);
    NU_INIT_SERIAL(nu_nu32_serial2, &args2);
    nu_nu32_serial1 = nu_nu32_serial;
}

static ALWAYSINLINE void
nu_nu32_setup_serial_v2(void)
{
    nu_init_serial_platform_args_t args1 = NU_SERIAL_UART1;
    nu_init_serial_platform_args_t args2 = NU_SERIAL_UART3;  /* = UART2A */
    NU_INIT_SERIAL(nu_nu32_serial, &args1);
    NU_INIT_SERIAL(nu_nu32_serial2, &args2);
    nu_nu32_serial1 = nu_nu32_serial;
}

 #pragma GCC diagnostic error "-Wdeprecated-declarations"

void
nu_nu32_setup_serial(nu_nu32_version_t version)
{
    switch (version) {
    case NU_NU32_V1:
        nu_nu32_setup_serial_v1();
        break;
    case NU_NU32_V2:
        nu_nu32_setup_serial_v2();
        break;
    default:
        return;
    }
    nu_serial_setup(nu_nu32_serial1, 115200, &nu_serial_platform_setup_defaults);
    nu_serial_setup(nu_nu32_serial2, 115200, &nu_serial_platform_setup_defaults);
}

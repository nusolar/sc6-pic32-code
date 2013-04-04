#include "nu32.h"
#include "nu_types.h"
#include "utility.h"
#include <peripheral/adc10.h>
#include <peripheral/cvref.h>
#include <peripheral/eth.h>
#include <peripheral/pmp.h>
#include <peripheral/system.h>

enum nu_nu32_version nu_nu32_version = NU_NU32_V1;

const char *nu_nu32_versions[] = {
#define NU_NU32_VERSION(v) STRINGIZE(v),
    NU_NU32_VERSIONS
#undef NU_NU32_VERSION
};

static struct nu_led _nu32_led0;
static struct nu_led _nu32_led1;
static struct nu_led _nu32_led2;
static struct nu_pin _nu32_switch;
static struct nu_serial _nu32_serial;
static struct nu_serial _nu32_serial1;
static struct nu_serial _nu32_serial2;

struct nu_led *nu_nu32_led0 = &_nu32_led0;
struct nu_led *nu_nu32_led1 = &_nu32_led1;
struct nu_led *nu_nu32_led2 = &_nu32_led2;
struct nu_pin *nu_nu32_switch = &_nu32_switch;
struct nu_serial *nu_nu32_serial = &_nu32_serial;
struct nu_serial *nu_nu32_serial1 = &_nu32_serial1;
struct nu_serial *nu_nu32_serial2 = &_nu32_serial2;

void
nu_nu32_setup_leds(void)
{
    nu_led_setup(nu_nu32_led1);
    nu_led_setup(nu_nu32_led2);
}

 #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static void
nu_nu32_setup_version_specific(enum nu_nu32_version version)
{
    nu_nu32_version = version;

    switch (version) {
    case NU_NU32_V1:
        nu_nu32_led0 = nu_nu32_led2;
        NU_INIT_LED(nu_nu32_led1, IOPORT_A, BIT_4);
        NU_INIT_LED(nu_nu32_led2, IOPORT_A, BIT_5);

        NU_INIT_PIN(nu_nu32_switch, IOPORT_C, BIT_13);

        NU_INIT_SERIAL(nu_nu32_serial, UART1);
        nu_nu32_serial1 = nu_nu32_serial;
        NU_INIT_SERIAL(nu_nu32_serial2, UART4); /* used by bootloader */
        break;
    case NU_NU32_V2:
        NU_INIT_LED(nu_nu32_led0, IOPORT_G, BIT_12);
        NU_INIT_LED(nu_nu32_led1, IOPORT_G, BIT_13);
        nu_nu32_led2 = nu_nu32_led0;

        NU_INIT_PIN(nu_nu32_switch, IOPORT_G, BIT_6);

        NU_INIT_SERIAL(nu_nu32_serial, UART1);
        nu_nu32_serial1 = nu_nu32_serial;
        NU_INIT_SERIAL(nu_nu32_serial2, UART3);
        break;
    case NU_NUM_NU32_VERSIONS:
    default:
        break;
    }
}

 #pragma GCC diagnostic error "-Wdeprecated-declarations"

COLD WEAK CONSTRUCTOR(()) void
nu_nu32_setup(enum nu_nu32_version version, unsigned long hz)
{
    nu_hz = hz;

    nu_nu32_setup_version_specific(version);

    SYSTEMConfig(hz, SYS_CFG_ALL);
    /* configure for multi-vectored mode */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    /* enable multi-vector interrupts */
    INTEnableSystemMultiVectoredInt();
    /* disable JTAG to get A4 and A5 back */
    DDPCONbits.JTAGEN = 0;
    /* disable comparator in attempt to get A9 and A10 back */
    CVREFOpen(CVREF_DISABLE);
    /* disable ADC in attempt to get A9 and A10 back...possibly only need the
     * second config to do this
     */
#define my_OpenADC10(config1, config2, config3, configport, configscan) (mPORTBSetPinsAnalogIn(configport), AD1CSSL = (unsigned)~(configscan), AD1CON3 = (config3), AD1CON2 = (config2), AD1CON1 = (config1) )
    my_OpenADC10(ADC_MODULE_OFF, ADC_VREF_AVDD_AVSS, 0, 0, 0);
    /* disable ethernet controller */
    EthEnable(0);
    /* disable parallel port */
    mPMPDisable();

    /* Configure LED outputs */
    nu_nu32_setup_leds();

    /* Configure input for switch */
    nu_pin_set_digital_in(nu_nu32_user);
}

#include "nu/platform/nu32.h"
#include "nu/param.h"
#include "nu/utility.h"
#include "nu/types.h"
#include "nu/platform.h"

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32.c"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

#include <peripheral/adc10.h>
#include <peripheral/cvref.h>
#include <peripheral/eth.h>
#include <peripheral/pmp.h>
#include <peripheral/system.h>

static struct nu__Pin _nu32_switch;
struct nu__Pin *nu__Nu32__switch = &_nu32_switch;

#if 0

static void
nu__Nu32__setup_pinctl(void)
{
    switch (nu32_version) {
    case NU_NU32_V1:
        NU_INIT_PIN(nu__Nu32__switch, IOPORT_C, BIT_13);
        break;
    case NU_NU32_V2:
        NU_INIT_PIN(nu__Nu32__switch, NU_PIN_G6);
        break;
    default:
        return;
    }
    /* Configure input for switch */
    nu__Pin__set_digital_in(nu__Nu32__user);
}
#endif

void
nu__Nu32__setup(UNUSED nu__Nu32__Version_t version, nu__Param__Hz_t hz)
{
    NU_HZ = hz;

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
}


#endif /* NU_PLATFORM switch */

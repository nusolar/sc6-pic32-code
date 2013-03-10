#include "nu32.h"
#include "nu_types.h"
#include <peripheral/adc10.h>
#include <peripheral/cvref.h>
#include <peripheral/eth.h>
#include <peripheral/pmp.h>
#include <peripheral/system.h>

void
nu32_setup_leds(void)
{
    nu_led_setup(&nu32_led0);
    nu_led_setup(&nu32_led1);
}

void
nu32_setup(void)
{
    SYSTEMConfig(HZ, SYS_CFG_ALL);
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
    nu32_setup_leds();

    /* Configure input for switch */
    nu_pin_set_digital_in(&nu32_user);
}

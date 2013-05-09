#ifndef NU_NU32_LED_H
#define NU_NU32_LED_H 1

#include "nu/led.h"
#include "nu/platform/nu32.h"

/* recommendation: DON'T use the combination nu_nu32_led0 and nu_nu32_led2 */
DEPRECATED extern struct nu_led *nu_nu32_led0;
extern struct nu_led *nu_nu32_led1;
extern struct nu_led *nu_nu32_led2;

void
nu_nu32_setup_led(nu_nu32_version_t version);

#endif

#ifndef NU_NU32_H
#define NU_NU32_H

#include "compiler.h"
#include "led.h"
#include "param.h"
#include "pinctl.h"
#include "serial.h"

#if NU32_V == 1
static const LED(nu32_led1,    IOPORT_A, BIT_4);
static const LED(nu32_led2,    IOPORT_A, BIT_5);
static const struct led nu32_led0 ALIAS("nu32_led2");   /* preserve code compatability */
static const PIN(nu32_switch,  IOPORT_C, BIT_13);
static const SERIAL(nu32_serial, UART1);
static const struct serial nu32_serial1 ALIAS("nu32_serial");
static const SERIAL(nu32_serial2, UART4);  /* used by bootloader */
#elif NU32_V == 2
static const LED(nu32_led0,    IOPORT_G, BIT_12);
static const LED(nu32_led1,    IOPORT_G, BIT_13);
static const struct led nu32_led2 ALIAS("nu32_led0");   /* preserve code compatability */
static const PIN(nu32_switch,  IOPORT_G, BIT_6);
static const SERIAL(nu32_serial, UART3);   /* = UART2A */
static const struct serial nu32_serial1 ALIAS("nu32_serial");
#endif

static const struct pin nu32_user ALIAS("nu32_switch");

WEAK CONSTRUCTOR() void
nu32_setup(void);

void
nu32_setup_leds(void);

#endif

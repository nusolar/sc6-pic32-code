#define inline __inline__

#include "compiler.h"
#include "led.h"
#include "nu_types.h"
#include "pinctl.h"

s32
main(void)
{
    PIN(pin1, IOPORT_A, BIT_2);
    LED(led1, IOPORT_A, BIT_3);
    
    pin_set_digital_out(&pin1);
    pin_set(&pin1);
    pin_clear(&pin1);
    pin_toggle(&pin1);
    INIT_PIN(&pin1, IOPORT_B, BIT_3);

    led_on(&led1);
    
    return 0;
}

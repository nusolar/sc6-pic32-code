#include "nu/board/drivercontrols.h"

#define NU_DC_PIN_INIT(name, ltr, num)   self->name = (struct nu__DriverControls) NU_PIN_INIT(NU_PIN_PLATFORM_INIT(ltr, num));
#define NU_DC_ANA_INIT(name, ltr, num)   nu__Pin__set_analog_out(&(self->name));
#define NU_DC_DIGI_INIT(name, ltr, num)  nu__Pin__set_digital_in(&(self->name));
#define NU_DC_DIGI_OUT(name, ltr, num) nu__Pin__set_digital_out(&(self->name)); nu__Pin__clear(&(self->name));


void
nu__DriverControls__setup_pins(struct nu__DriverControls *self)
{
//	ANALOG_INS(NU_DC_PIN_INIT)

    ANALOG_INS(NU_DC_ANA_INIT)
    DIGITAL_INS(NU_DC_DIGI_INIT)
    DIGITAL_OUTS(NU_DC_DIGI_OUT)
}

s32
nu__DriverControls__main(void)
{
	struct nu__DriverControls self;
    nu__DriverControls__setup_pins(&self);
    // read pins
    // assemble DC CAN struct
    // send
    // send acceleration / power CAN pkt to motor controller
    // Draw status on Nokia
    // Listen for commands, implement them
    return 0;
}

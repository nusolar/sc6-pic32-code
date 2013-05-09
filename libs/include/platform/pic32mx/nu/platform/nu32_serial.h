#ifndef NU32_PIC32MX_SERIAL_H
#define NU32_PIC32MX_SERIAL_H 1

#include "nu/serial.h"
#include "nu/platform/nu32.h"

DEPRECATED extern struct nu_serial *nu_nu32_serial;
extern struct nu_serial *nu_nu32_serial1;
extern struct nu_serial *nu_nu32_serial2;

void
nu_nu32_setup_serial(nu_nu32_version_t version);

#endif

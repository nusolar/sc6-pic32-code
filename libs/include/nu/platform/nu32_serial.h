#ifndef NU_PLATFORM_NU32_SERIAL_H
#define NU_PLATFORM_NU32_SERIAL_H 1

#include "nu/peripheral/serial.h"
#include "nu/platform/nu32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32_serial.h"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

DEPRECATED extern struct nu_serial *nu_nu32_serial;
extern struct nu_serial *nu_nu32_serial1;
extern struct nu_serial *nu_nu32_serial2;

void
nu_nu32_setup_serial(nu_nu32_version_t version);

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_NU32_SERIAL_H */

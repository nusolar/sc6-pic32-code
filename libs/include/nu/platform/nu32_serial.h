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

DEPRECATED extern struct nu__Serial *nu__Nu32__serial;
extern struct nu__Serial *nu__Nu32__serial1;
extern struct nu__Serial *nu__Nu32__serial2;

void
nu__Nu32__setup_serial(nu__Nu32__Version_t version);

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_NU32_SERIAL_H */

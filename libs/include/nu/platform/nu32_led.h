#ifndef NU_PLATFORM_NU32_LED_H
#define NU_PLATFORM_NU32_LED_H 1

#include "nu/component/led.h"
#include "nu/platform/nu32.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/nu32_led.h"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

/* recommendation: DON'T use the combination nu__Nu32__led0 and nu__Nu32__led2 */
DEPRECATED extern struct nu__Led *nu__Nu32__led0;
extern struct nu__Led *nu__Nu32__led1;
extern struct nu__Led *nu__Nu32__led2;

void
nu__Nu32__setup_led(nu__Nu32__Version_t version);

#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_NU32_LED_H */

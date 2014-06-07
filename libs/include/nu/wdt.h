#ifndef NU_WDT_H
#define NU_WDT_H 1

#include "nu/platform.h"
#include "nu/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif


#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in nu/platform/wdt.h!"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX

#include <xc.h>
#include <peripheral/wdt.h>

static ALWAYSINLINE
void nu_platform_wdt_clearable(void)	{ClearWDT();}

static ALWAYSINLINE
void nu__WDT__enable(void)	{EnableWDT();}

static ALWAYSINLINE
void nu__WDT__disable(void)	{DisableWDT();}

#endif /* NU_PLATFORM switch */


/**
 * Clear WatchDogTimer
 */
extern void (*nu__WDT__clear) (void);

void
nu__WDT__clear_enable(void);

void
nu__WDT__clear_disable(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

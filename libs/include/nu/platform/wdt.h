/*
 * File:   wdt.h
 * Author: alex
 *
 * Created on July 7, 2013, 10:07 AM
 */

#ifndef NU_PLATFORM_WDT_H
#define	NU_PLATFORM_WDT_H

#include "nu/compiler.h"
#include "nu/platform.h"

#ifdef	__cplusplus
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

#ifdef	__cplusplus
} /* extern "C" */
#endif

#endif	/* NU_PLATFORM_WDT_H */


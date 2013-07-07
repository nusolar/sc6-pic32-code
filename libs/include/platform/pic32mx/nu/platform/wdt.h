/* 
 * File:   wdt.h
 * Author: alex
 *
 * Created on July 7, 2013, 10:07 AM
 */

#ifndef PLATFORM_WDT_H
#define	PLATFORM_WDT_H

#include "nu/compiler.h"

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <peripheral/wdt.h>


static ALWAYSINLINE
void nu_platform_wdt_clearable(void)	{ClearWDT();}

static ALWAYSINLINE
void nu_wdt_enable(void)	{EnableWDT();}

static ALWAYSINLINE
void nu_wdt_disable(void)	{DisableWDT();}


#ifdef	__cplusplus
}
#endif

#endif	/* PLATFORM_WDT_H */


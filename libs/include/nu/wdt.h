#ifndef NU_WDT_H
#define NU_WDT_H 1

#include <xc.h>
#include <peripheral/wdt.h>

/**
 * Clear WatchDogTimer
 */
extern void (*nu_wdt_clear) (void);

void
nu_wdt_enable_clear(void);

void
nu_wdt_disable_clear(void);

#endif

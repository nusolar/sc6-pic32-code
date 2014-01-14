#ifndef NU_WDT_H
#define NU_WDT_H 1

#include "nu/platform/wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

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

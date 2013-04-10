#ifndef NU_WDT_H
#define NU_WDT_H 1

/**
 * Clear WatchDogTimer
 */
extern void (*nu_wdt_clear) (void);

void
nu_wdt_enable_clear(void);

void
nu_wdt_disable_clear(void);

#endif

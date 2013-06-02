#ifndef NU_WDT_H
#define NU_WDT_H 1

/**
 * Clear WatchDogTimer
 */
extern void (*nu_wdt_clear) (void);

void
nu_wdt_clear_enable(void);

void
nu_wdt_clear_disable(void);

#endif

#ifndef NU_WDT_H
#define NU_WDT_H

extern void (*clear_wdt)(void);

void
enable_clear_wdt(void);

void
disable_clear_wdt(void);

#endif

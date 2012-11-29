#ifndef NU_WDT_H
#define NU_WDT_H

#include <plib.h>

extern void (*clear_wdt) (void);

void
enableClearWdt(void);

void
disableClearWdt(void);

#endif

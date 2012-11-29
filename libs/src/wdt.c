#include "../include/wdt.h"

static void
enabled_clear_wdt(void)
{
    ClearWDT();
}

static void
disabled_clear_wdt(void){}

void (*clear_wdt) (void) = enabled_clear_wdt;

void
enableClearWdt(void)
{
    clear_wdt = enabled_clear_wdt;
}

void
disableClearWdt(void)
{
    clear_wdt = disabled_clear_wdt;
}

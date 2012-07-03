#include "../include/wdt.h"

static void
enabledClearWdt(void)
{
    ClearWDT();
}

static void
disabledClearWdt(void){}

void (*CLEARWDT) (void) = enabledClearWdt;

void
enableClearWdt(void)
{
    CLEARWDT = enabledClearWdt;
}

void
disableClearWdt(void)
{
    CLEARWDT = disabledClearWdt;
}

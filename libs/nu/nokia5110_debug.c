#include "../include/nu/nokia5110_debug.h"

const struct nokia5110 *debugScreen;

inline void
setDebugScreen (const struct nokia5110 *np)
{
    debugScreen = np;
}

void
printDebug (const char *file, int line)
{
    debugScreen->op->gotoXY(debugScreen, 0, 3);
    debugScreen->op->printf(debugScreen, "File:       ");
    debugScreen->op->gotoXY(debugScreen, 0, 4);
    debugScreen->op->printf(debugScreen, "%s            ", file);
    debugScreen->op->gotoXY(debugScreen, 0, 5);
    debugScreen->op->printf(debugScreen, "Line %d      ", line);
}

void
printAssert (const char *test, const char *file, int line, int passed)
{
    debugScreen->op->gotoXY(debugScreen, 0, 2);
    if (passed)
        debugScreen->op->printf(debugScreen, "ASSERT PASS: ");
    else
        debugScreen->op->printf(debugScreen, "ASSERT FAIL: ");
    debugScreen->op->gotoXY(debugScreen, 0, 3);
    debugScreen->op->printf(debugScreen, "%s            ", file);
    debugScreen->op->gotoXY(debugScreen, 0, 4);
    debugScreen->op->printf(debugScreen, "Line %d      ", line);
    debugScreen->op->gotoXY(debugScreen, 0, 5);
    debugScreen->op->printf(debugScreen, "%s            ", test);
}

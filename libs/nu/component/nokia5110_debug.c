#include "nu/component/nokia5110_debug.h"

const struct nu_nokia5110 *debugScreen;

inline void
setDebugScreen (const struct nu_nokia5110 *np)
{
    debugScreen = np;
}

void
printDebug (const char *file, int line)
{
    nu_nokia_goto_xy(debugScreen, 0, 3);
    nu_nokia_printf(debugScreen, "File:       ");
    nu_nokia_goto_xy(debugScreen, 0, 4);
    nu_nokia_printf(debugScreen, "%s            ", file);
    nu_nokia_goto_xy(debugScreen, 0, 5);
    nu_nokia_printf(debugScreen, "Line %d      ", line);
}

void
printAssert (const char *test, const char *file, int line, int passed)
{
    nu_nokia_goto_xy(debugScreen, 0, 2);
    if (passed)
        nu_nokia_printf(debugScreen, "ASSERT PASS: ");
    else
        nu_nokia_printf(debugScreen, "ASSERT FAIL: ");
    nu_nokia_goto_xy(debugScreen, 0, 3);
    nu_nokia_printf(debugScreen, "%s            ", file);
    nu_nokia_goto_xy(debugScreen, 0, 4);
    nu_nokia_printf(debugScreen, "Line %d      ", line);
    nu_nokia_goto_xy(debugScreen, 0, 5);
    nu_nokia_printf(debugScreen, "%s            ", test);
}

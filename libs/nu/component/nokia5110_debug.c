#include "nu/component/nokia5110_debug.h"

const struct nu__Nokia5110 *debugScreen;

inline void
nu__Nokia__set_debug_screen (const struct nu__Nokia5110 *np)
{
    debugScreen = np;
}

void
nu__Nokia__print_debug (const char *file, int line)
{
    nu__Nokia__goto_xy(debugScreen, 0, 3);
    nu__Nokia__printf(debugScreen, "File:       ");
    nu__Nokia__goto_xy(debugScreen, 0, 4);
    nu__Nokia__printf(debugScreen, "%s            ", file);
    nu__Nokia__goto_xy(debugScreen, 0, 5);
    nu__Nokia__printf(debugScreen, "Line %d      ", line);
}

void
nu__Nokia__print_assert (const char *test, const char *file, int line, int passed)
{
    nu__Nokia__goto_xy(debugScreen, 0, 2);
    if (passed)
        nu__Nokia__printf(debugScreen, "ASSERT PASS: ");
    else
        nu__Nokia__printf(debugScreen, "ASSERT FAIL: ");
    nu__Nokia__goto_xy(debugScreen, 0, 3);
    nu__Nokia__printf(debugScreen, "%s            ", file);
    nu__Nokia__goto_xy(debugScreen, 0, 4);
    nu__Nokia__printf(debugScreen, "Line %d      ", line);
    nu__Nokia__goto_xy(debugScreen, 0, 5);
    nu__Nokia__printf(debugScreen, "%s            ", test);
}

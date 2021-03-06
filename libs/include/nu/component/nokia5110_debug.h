#ifndef NU_NOKIA5110_DEBUG_H
#define NU_NOKIA5110_DEBUG_H 1

#include "nu/component/nokia5110.h"

/**
 * @def PRINTDEBUG()
 * Prints the file name and line number to Nokia5110 LCD screen lines 2, 3, 4, and 5.
 *
 * For example,
 * @code
 * PRINTDEBUG(); // line 1337 in file example.c
 * @endcode
 * Will print:
 * @verbatim



File:
example.c
Line 1337
@endverbatim
 *
 * to the lcd screen.
 */
#define PRINTDEBUG() do {                                   \
                            nu__Nokia__print_debug(__FILE__, __LINE__); \
                        } while(0)

/**
 * @def ASSERTLCD(x)
 * Tests conditional expression \a x for TRUE or non-zero. If test is successful
 * (TRUE or non-zero), then a message stating that the assertion passed will
 * be printed to the first line of the lcd screen. Otherwise, a message stating
 * that the assertion failed will be printed to the first line of the lcd screen.
 * In both cases, the expression \a x that either
 * failed or passed will be printed to the second line of the lcd screen.
 *
 * @param[in]   x   The conditional test to evaluate.
 *
 * For example,
 * @code
 * ASSERTLCD(1==0)
 * @endcode
 * will print
 * @verbatim




ASSERT FAILED:
1==0
@endverbatim
 * to the lcd screen.
 *
 */
#define ASSERTLCD(x)    do {                    \
                            printAssert(#x, __FILE__, __LINE__, x); \
                        }while(0)

INLINE void
nu__Nokia__set_debug_screen (const struct nu__Nokia5110 *n);

void
nu__Nokia__print_debug (const char *file, int line);

void
nu__Nokia__print_assert (const char *test, const char *file, int line, int passed);

#endif

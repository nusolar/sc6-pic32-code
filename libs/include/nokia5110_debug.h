/**
 * @file   LcdDebug.h
 * @author Chris Yungmann (christopheryungmann2010@u.northwestern.edu)
 * @date   December, 2011
 * @brief Macro functions for printing debugging information to the lcd screen (Nokia5110).
 */

#ifndef __NU_NOKIA5110_DEBUG_H
#define __NU_NOKIA5110_DEBUG_H

#include "nokia5110.h"

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
                            printDebug(__FILE__, __LINE__); \
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

inline void
setDebugScreen (const struct nokia5110 *n) __attribute__((always_inline));

void
printDebug (const char *file, int line);

void
printAssert (const char *test, const char *file, int line, int passed);

#endif

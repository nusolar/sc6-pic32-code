#ifndef NU_MINUNIT_H
#define NU_MINUNIT_H

/**
 * @file
 * minunit simple unit testing framework
 * 
 * There's some slight modifications from the original minunit
 * to fix some compilation warnings.
 * 
 * @sa http://www.jera.com/techinfo/jtns/jtn002.html
 */

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
extern unsigned int tests_run;

#endif

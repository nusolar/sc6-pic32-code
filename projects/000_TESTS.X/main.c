#include "common_pragmas.h"
#include "minunit.h"
#include "nu32.h"
#include "nu_types.h"
#include "pinctl.h"

unsigned int tests_run = 0;

static const char *
test_example(void)
{
    mu_assert("error, 3 != 3!", 3 == 3);
    return NULL;
}

static const char *
test_pinctl(void)
{
    return NULL;
}

static const char *
all_tests(void)
{
    mu_run_test(test_example);
    mu_run_test(test_pinctl);
    return NULL;
}

s32
main(void)
{
    const char *result;
    if ((result = all_tests()))
        puts(result);
    else
        puts("ALL TESTS PASSED");
    printf("Tests run: %d\n", tests_run);

    while (1)
        Nop();
    return 0;
}

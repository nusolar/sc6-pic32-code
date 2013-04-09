#ifndef NU_UTILITY_H
#define NU_UTILITY_H 1

#include <stdlib.h>
#include "utility/arith.h"
#include "utility/bits.h"
#include "utility/data.h"
#include "utility/preprocessor.h"
#include "utility/static.h"
#include "compiler.h"

/* for loop that shouldn't be optimized away
 * it wouldn't hurt to also declare the iteration variable
 * as volatile, though this can introduce overhead,
 * making the loop slower
 */
#define BUSY_FOR(init, cond, iter)  \
    for (init, cond, iter) ASM VOLATILE("")

#define CLAIM_PIN(n)                \
    void claimed_pin_##n(void);     \
    void claimed_pin_##n(void){}

#endif

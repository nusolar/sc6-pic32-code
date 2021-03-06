#ifndef NU_UTILITY_H
#define NU_UTILITY_H 1

#include <stdlib.h>
#include "nu/utility/arith.h"
#include "nu/utility/bits.h"
#include "nu/utility/data.h"
#include "nu/utility/preprocessor.h"
#include "nu/utility/static.h"

#define NU_NOP()    ASM VOLATILE("")

/* for loop that shouldn't be optimized away
 * it wouldn't hurt to also declare the iteration variable
 * as volatile, though this can introduce overhead,
 * making the loop slower
 */
#define BUSY_FOR(init, cond, iter)  \
    do { for (init, cond, iter) NU_NOP(); }while(0)

#define CLAIM_PIN(n)                \
    void claimed_pin_##n(void);     \
    void claimed_pin_##n(void){}

#endif


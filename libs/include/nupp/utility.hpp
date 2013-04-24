#ifndef NUPP_UTILITY_HPP
#define NUPP_UTILITY_HPP 1

#include <stdlib.h>
#include "nupp/utility/arith.hpp"
#include "nupp/utility/bits.hpp"
#include "nupp/utility/data.hpp"
#include "nupp/utility/preprocessor.hpp"
#include "nupp/utility/static.hpp"
#include "nupp/compiler.hpp"

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

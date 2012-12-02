#ifndef __NU_TIMER_H
#define __NU_TIMER_H

#ifdef PC_BUILD
    #include "pctime.h"
#else
    #include <plib.h>
#endif

#include <math.h>

#include "compiler.h"
#include "nu32.h"
#include "utility.h"

INLINE void
resetTimer(void);

double
readTimer(void);

INLINE CONST double
ticksToSecs(uint32_t ticks);

INLINE CONST uint32_t
secsToTicks(double secs);

void
delay(double seconds);

INLINE void
delay_us(double usec);

INLINE void
delay_ms(double msec);

#endif

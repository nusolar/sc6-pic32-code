#ifndef __NU_TIMER_H
#define __NU_TIMER_H

#ifdef PC_BUILD
    #include "pctime.h"
#else
    #include <plib.h>
#endif

#include "nu32.h"
#include "utility.h"

inline void
resetTimer(void);

double
readTimer(void);

inline double
ticksToSecs(uint32_t ticks);

inline uint32_t
secsToTicks(double secs);

void
delay(double seconds);

void
delay_us(double usec);

void
delay_ms(double msec);

#endif
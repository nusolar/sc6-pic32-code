#include "../include/timer.h"

/* HAL for PC build */
#ifdef PC_BUILD
    static uint32_t coretimer = 0;
    static unsigned long long lastMsTime = 0;

    static void
    WriteCoreTimer (uint32_t val);

    static uint32_t
    ReadCoreTimer (void);
#endif

INLINE void
resetTimer(void)
{
    WriteCoreTimer(0);
}

INLINE double
readTimer(void)
{
    return ticksToSecs(ReadCoreTimer());
}

INLINE CONST double
ticksToSecs(uint32_t ticks)
{
    /* 1 core tick = 2 SYS cycles */
    return (((double)ticks*2.0)/(double)sys_clk_hz);
}

INLINE CONST uint32_t
secsToTicks(double secs)
{
    return (uint32_t)((secs*(double)sys_clk_hz)/2.0);
}

void
delay(double seconds)
{
    /* @TODO: formerly unsigned ints; see if this still works */
    unsigned int tWait  = (uint32_t)(secsToTicks(fabs(seconds)));
    unsigned int tStart = ReadCoreTimer();

    while ((ReadCoreTimer()-tStart) < tWait)
        ; /* do nothing */
}

inline void
delay_us(double usec)
{
    delay(usec/1E6);
}

inline void
delay_ms(double msec)
{
    delay(msec/1E3);
}

/* HAL for PC build */
#ifdef PC_BUILD
    static void
    WriteCoreTimer(uint32_t val)
    {
        coretimer = 0;
        lastMsTime = 0;
    }

    static uint32_t
    ReadCoreTimer(void)
    {
        unsigned long long currentMsTime = GetTimeMs64();
        unsigned long long dt = currentMsTime - lastMsTime;
        lastMsTime = currentMsTime;
        coretimer += (uint32_t)(((double)dt/1.0E3)*sys_clk_hz/2.0);

        return coretimer;
    }
#endif
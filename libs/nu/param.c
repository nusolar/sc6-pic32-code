#include "nu/param.h"

nu__Param__Hz_t nu__Param__Hz = NU_DEFAULT_HZ;



#include "nu/timer.h"

uint32_t _nu__Timer__carry = 0;
void __ISR(_CORE_TIMER_VECTOR, ipl2) CoreTimerHandler(void)
{
	++_nu__Timer__carry;
	//UpdateCoreTimer(CORE_TICK_RATE);
	mCTClearIntFlag();
}

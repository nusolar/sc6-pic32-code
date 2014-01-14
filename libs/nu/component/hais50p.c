#include "nu/component/hais50p.h"

static INLINE float
nu__Hais50p__voltage_to_current(float voltage)
{
    /* V_out = V_ref + (0.625*I_p/(I_pn))
     * V_ref = 2.5 +/- 0.025V
     * I_pn  = primary nominal curent = 50A for HAIS-50P
     * I_p   = primary current (measured current) */
    return ((voltage - 2.5f) * 80);
}

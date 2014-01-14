#ifndef NU_CAN_MPPT_H
#define NU_CAN_MPPT_H 1

#include "nu/utility.h"

union can_mppt {
    union can_mppt_tx {
        struct can_mppt_tx_frame {
            unsigned    bvlr :1;    /* battery voltage level reached:
                                     * 0 = Uout < Uoutmax;
                                     * 1 = Uout = Uoutmax
                                     */
            unsigned    ovt :1;     /* over temperature flag:
                                     * 0 = Tcooler < 95C;
                                     * 1 = Tcooler >= 95C
                                     */
            unsigned    noc :1;     /* no charge flag:
                                     * 0 = battery connected;
                                     * 1 = no battery connected
                                     */
            unsigned    undv :1;    /* under voltage flag:
                                     * 0 = Uin > 26V;
                                     * 1 = Uin <= 26V
                                     */
            unsigned    reserved :2;
            signed      uIn :10;
            unsigned    reserved2 :6;
            unsigned    iIn :10;
            unsigned    reserved3 :6;
            unsigned    uOut :10;
            unsigned    temp :8;    /* only accurate to steps of 5C */
        } __attribute__((packed)) frame;
    } tx;
};

STATIC_ASSERT(sizeof(struct can_mppt_tx_frame) == 7, SIZE_MISMATCH);

#endif

#ifndef NU_CAN_COMMON_H
#define NU_CAN_COMMON_H

union can_common;
union can_common_tx;

struct can_common_tx_err_priority_num {
    int32_t     priority;
    int32_t     errNum;
};

union can_common_tx {
    struct can_common_tx_err_priority_num   errPriorityNum;
};

union can_common {
    union can_common_tx tx;
};

#endif
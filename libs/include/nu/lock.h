#ifndef NU_LOCK_H
#define NU_LOCK_H 1

#define nu_lock_acquire(lock) \
    __sync_lock_test_and_set(lock, 1)

#define nu_lock_release(lock) \
    __sync_lock_release(lock)

typedef struct {
    volatile unsigned int lock;
} spinlock_t;

#endif

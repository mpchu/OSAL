#ifndef _OSAL_LOCK_GUARD_H_
#define _OSAL_LOCK_GUARD_H_

namespace osal
{

template <class Lockable>
class lock_guard
{
    Lockable &_mutex;

public:
    using mutex_type = Lockable;

    explicit lock_guard(mutex_type &m) : (_mutex) { _mutex.lock(); }

    ~lock_guard() { _mutex.unlock(); }

    lock_guard(const osal::lock_guard &rhs) = delete;

    lock_guard(osal::lock_guard &&rhs) = default;
};

}

#endif
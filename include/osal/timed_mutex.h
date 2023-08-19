#ifndef _OSAL_TIMED_MUTEX_H_
#define _OSAL_TIMED_MUTEX_H_

#include <chrono>

namespace osal
{

template <typename handle_t>
class timed_mutex
{
    handle_t _mut_handle;

public:
    using native_handle_type = handle_t;

    timed_mutex() = default;

    ~timed_mutex() = default;

    timed_mutex(const osal::timed_mutex &rhs) = default;

    timed_mutex(osal::timed_mutex &&rhs) = default;

    osal::timed_mutex &operator=(const osal::timed_mutex &rhs) = default;

    void lock();

    bool try_lock();

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period> &timeout_duration);

    template <class Clock, class Duration>
    bool try_lock_until(const std::chrono::time_point<Clock, Duration> &absolute_time);

    void unlock();

    timed_mutex::native_handle_type native_handle() { return _mut_handle; }
};

}

#endif
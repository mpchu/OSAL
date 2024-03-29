#ifndef _OSAL_INCLUDE_OSAL_OS_MUTEX_H_
#define _OSAL_INCLUDE_OSAL_OS_MUTEX_H_

#include <chrono>
#include "osal/osal_config.h"
#include "osal/os_clock.h"

namespace osal
{

namespace details
{

template <typename handle_t = configOSAL_MUTEX_NATIVE_HANDLE>
class os_mutex
{
    handle_t _handle;

public:
    using native_handle_type = handle_t;

    os_mutex();

    ~os_mutex();

    native_handle_type native_handle() const { return _handle; }

    void lock();

    bool try_lock();

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_try_lock_for(std::chrono::duration_cast<osal::chrono::ticks>(timeout));
    }

    void unlock();

private:
    // Delete copy & move
    os_mutex(const os_mutex &rhs) = delete;
    os_mutex(os_mutex &&rhs) = delete;

    bool impl_try_lock_for(const osal::chrono::ticks &timeout);
};

} // namespace details

using mutex = details::os_mutex<>;

} // namespace osal

#endif
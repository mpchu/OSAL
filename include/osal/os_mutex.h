#ifndef _OSAL_OS_MUTEX_H_
#define _OSAL_OS_MUTEX_H_

#include <chrono>
#include "osal/osal_config.h"

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

    native_handle_type native_handle() { return _handle; }

    void lock();

    bool try_lock();

    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_try_lock_for(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout));
    }

    void unlock();

private:
    bool impl_try_lock_for(const std::chrono::nanoseconds &timeout);
};

} // namespace details

using mutex = details::os_mutex<>;

} // namespace osal

#endif
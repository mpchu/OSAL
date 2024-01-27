#ifndef _OSAL_OS_MUTEX_H_
#define _OSAL_OS_MUTEX_H_

#include <chrono>
#include "osal/osal_config.h"

namespace osal
{

namespace api
{

template <typename handle_t = configOSAL_MUTEX_NATIVE_HANDLE>
class os_mutex
{
    handle_t _handle;

public:
    using native_handle_type = handle_t;

    os_mutex();

    ~os_mutex();

    os_mutex::native_handle_type native_handle() { return _handle; }

    void lock();

    bool try_lock();

    bool try_lock_for(const std::chrono::milliseconds &timeout);

    void unlock();
};

}

using mutex = api::os_mutex<>;

} // namespace osal

#endif
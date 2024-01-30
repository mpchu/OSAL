#ifndef _OSAL_OS_BINARY_SEMAPHORE_H_
#define _OSAL_OS_BINARY_SEMAPHORE_H_

#include <cstddef>
#include "osal/osal_config.h"

namespace osal
{

namespace details
{

template <typename handle_t = configOSAL_SEMAPHORE_NATIVE_HANDLE>
class os_binary_semaphore
{
    handle_t _handle;

public:
    using native_handle_type = handle_t;

    os_binary_semaphore(bool desired = false);

    os_binary_semaphore(const os_binary_semaphore &rhs) = delete;

    ~os_binary_semaphore();

    native_handle_type native_handle() { return _handle; }

    void release();

    void acquire();

    bool try_acquire();

    bool try_acquire_for(const std::chrono::milliseconds &timeout);

    constexpr std::size_t maximum() noexcept { return 1; }
};

} // namespace details

using binary_semaphore = details::os_binary_semaphore<>;

} // namespace osal

#endif
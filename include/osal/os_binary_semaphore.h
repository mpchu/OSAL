#ifndef _OSAL_INCLUDE_OSAL_OS_BINARY_SEMAPHORE_H_
#define _OSAL_INCLUDE_OSAL_OS_BINARY_SEMAPHORE_H_

#include <chrono>
#include <cstddef>
#include "osal/osal_config.h"
#include "osal/os_clock.h"

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

    os_binary_semaphore(bool desired = true);

    ~os_binary_semaphore();

    native_handle_type native_handle() { return _handle; }

    void release();

    void acquire();

    bool try_acquire();

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_try_acquire_for(const std::chrono::duration_cast<osal::chrono::ticks>(timeout));
    }

    constexpr std::size_t maximum() noexcept { return 1; }

private:
    // Delete copy & move
    os_binary_semaphore(const os_binary_semaphore &rhs) = delete;
    os_binary_semaphore(os_binary_semaphore &&rhs) = delete;

    bool impl_try_acquire_for(const osal::chrono::ticks &timeout);
};

} // namespace details

using binary_semaphore = details::os_binary_semaphore<>;

} // namespace osal

#endif
#ifndef _OSAL_INCLUDE_OSAL_OS_COUNTING_SEMAPHORE_H_
#define _OSAL_INCLUDE_OSAL_OS_COUNTING_SEMAPHORE_H_

#include <chrono>
#include <cstddef>
#include "osal/osal_config.h"

namespace osal
{

namespace details
{

template <typename handle_t = configOSAL_SEMAPHORE_NATIVE_HANDLE>
class os_counting_semaphore
{
    handle_t _handle;

    std::size_t _max_count;

public:
    using native_handle_type = handle_t;

    os_counting_semaphore(std::size_t max_count, std::size_t initial);

    os_counting_semaphore(const os_counting_semaphore &rhs) = delete;

    ~os_counting_semaphore();

    native_handle_type native_handle() { return _handle; }

    void release(std::size_t update = 1);

    void acquire();

    bool try_acquire();

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_try_acquire_for(const std::chrono::duration_cast<std::chrono::nanoseconds>(timeout));
    }

    constexpr std::size_t maximum() noexcept { return _max_count; }

private:
    bool impl_try_acquire_for(const std::chrono::nanoseconds &timeout);
};

} // namespace details

using counting_semaphore = details::os_counting_semaphore<>;

} // namespace osal

#endif
#ifndef _OSAL_INCLUDE_OSAL_OS_COUNTING_SEMAPHORE_H_
#define _OSAL_INCLUDE_OSAL_OS_COUNTING_SEMAPHORE_H_

#include <chrono>
#include <cstddef>
#include "osal/osal_config.h"
#include "osal/os_clock.h"

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

    os_counting_semaphore(std::size_t max_count) : os_counting_semaphore(max_count, max_count) {}

    ~os_counting_semaphore();

    native_handle_type native_handle() const { return _handle; }

    void release(std::size_t update = 1);

    void acquire();

    bool try_acquire();

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_try_acquire_for(const std::chrono::duration_cast<osal::chrono::ticks>(timeout));
    }

    std::size_t maximum() const noexcept { return _max_count; }

private:
    // Delete copy & move
    os_counting_semaphore(const os_counting_semaphore &rhs) = delete;
    os_counting_semaphore(os_counting_semaphore &&rhs) = delete;

    bool impl_try_acquire_for(const osal::chrono::ticks &timeout);
};

} // namespace details

using counting_semaphore = details::os_counting_semaphore<>;

} // namespace osal

#endif
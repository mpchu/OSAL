#ifndef _OSAL_OS_COUNTING_SEMAPHORE_H_
#define _OSAL_OS_COUNTING_SEMAPHORE_H_

#include <cstddef>
#include "osal/osal_config.h"

namespace osal
{

namespace api
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

    bool try_acquire_for(const std::chrono::milliseconds &timeout);

    constexpr std::size_t maximum() noexcept { return _max_count; }
};

}

using counting_semaphore = api::os_counting_semaphore<>;

}

#endif
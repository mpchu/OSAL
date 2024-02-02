#ifndef _OSAL_SOURCE_OS_PORTABLE_STD_INTERNAL_STD_SEMAPHORE_HANDLE_H_
#define _OSAL_SOURCE_OS_PORTABLE_STD_INTERNAL_STD_SEMAPHORE_HANDLE_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <mutex>

namespace osal
{

namespace details
{

class std_semaphore_handle
{
    std::mutex _mutex;

    std::condition_variable _condvar;

    const std::size_t _max_count;

    std::atomic_size_t _count;

public:
    std_semaphore_handle(std::size_t max_count, std::size_t initial);

    ~std_semaphore_handle() = default;
    
    void impl_release(std::size_t update = 1);

    void impl_acquire();

    bool impl_try_acquire();

    bool impl_try_acquire_for(const std::chrono::nanoseconds &timeout);
};

}

}


#endif
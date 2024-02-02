#include "os/portable/std/internal/std_semaphore_handle.h"

namespace osal
{

namespace details
{

std_semaphore_handle::std_semaphore_handle(std::size_t max_count, std::size_t initial)
    : _max_count(max_count),
      _count((max_count < initial) ? max_count : initial)
{
}

void std_semaphore_handle::impl_release(std::size_t update)
{
    for (std::size_t i = 0; i < update; ++i)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        ++_count;
        _condvar.notify_one();
    }
}

void std_semaphore_handle::impl_acquire()
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (_count == 0) { _condvar.wait(lock); } // Loop in case of spurious wake-ups
    --_count;
}

bool std_semaphore_handle::impl_try_acquire()
{
    std::unique_lock<std::mutex> lock(_mutex);
    if (_count)
    {
        --_count;
        return true;
    }
    return false;
}

bool std_semaphore_handle::impl_try_acquire_for(const std::chrono::nanoseconds &timeout)
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::cv_status status = _condvar.wait_for(lock, timeout);
    if (status == std::cv_status::no_timeout && _count)
    {
        --_count;
        return true;
    }
    return false;
}

}

}

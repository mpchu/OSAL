#ifndef _OSAL_COUNTING_SEMAPHORE_H_
#define _OSAL_COUNTING_SEMAPHORE_H_

#include <cstddef>

namespace osal
{

template <std::size_t max_count, typename handle_t>
class counting_semaphore
{
    handle_t _sem_handle;

public:
    using native_handle_type = handle_t;

    counting_semaphore(std::size_t initial);

    counting_semaphore(const counting_semaphore &rhs) = delete;

    ~counting_semaphore();

    void release(std::size_t update = 1);

    bool try_acquire();

    template <class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &timeout);

    template <class Clock, class Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration> &absolute_time);

    static constexpr std::size_t max() noexcept { return max_count; }
};

}

#endif
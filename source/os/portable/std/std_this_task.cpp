#include "osal/os_this_task.h"
#include <thread>

namespace osal
{

namespace this_task
{

void yield() noexcept
{
    std::this_thread::yield();
}

namespace details
{

void os_sleep_for(const osal::chrono::ticks &sleep_duration)
{
    std::this_thread::sleep_for(sleep_duration);
}

template <>
void os_sleep_until(const std::chrono::time_point<osal::chrono::steady_clock> &wakeup_time)
{
    std::this_thread::sleep_until(wakeup_time);
}

} // namespace details

} // namespace this_task

} // namespace osal

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

void os_sleep_for(const std::chrono::nanoseconds &sleep_duration)
{
    std::this_thread::sleep_for(sleep_duration);
}

} // namespace details

} // namespace this_task

} // namespace osal

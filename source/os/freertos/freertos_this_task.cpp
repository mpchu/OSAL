#include "osal/os_this_task.h"
#include "osal/os_clock.h"
#include "FreeRTOS.h"
#include "task.h"

namespace osal
{

namespace this_task
{

void yield() noexcept
{
    taskYIELD();
}

namespace details
{

void os_sleep_for(const osal::chrono::ticks &sleep_duration)
{
    vTaskDelay(sleep_duration.count());
}

template <>
void os_sleep_until(const std::chrono::time_point<osal::chrono::steady_clock> &wakeup_time)
{
    // Handle the case where wakeup_time has already passed
    const auto now = osal::chrono::steady_clock::now();
    const osal::chrono::ticks ticksToSleep = (wakeup_time > now)
                                             ? wakeup_time - now
                                             : osal::chrono::ticks::zero();

    TickType_t nativeNow = now.time_since_epoch().count();
    vTaskDelayUntil(&nativeNow, ticksToSleep.count());
}

} // namespace details

} // namespace this_task

} // namespace osal

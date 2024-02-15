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
void os_sleep_until(const std::chrono::time_point<osal::chrono::system_clock> &wakeup_time)
{
    const TickType_t wakeupTick = wakeup_time.time_since_epoch().count();
    TickType_t now = osal::chrono::system_clock::current_tick().count();

    // Handle the case where wakeup_time has already passed
    TickType_t ticksToSleep = (wakeupTick > now) 
                              ? wakeupTick - now
                              : 0;
    vTaskDelayUntil(&now, ticksToSleep);
}

} // namespace details

} // namespace this_task

} // namespace osal

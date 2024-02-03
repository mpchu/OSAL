#include "osal/os_this_task.h"
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

void os_sleep_for(const std::chrono::nanoseconds &sleep_duration)
{
    vTaskDelay(
        pdMS_TO_TICKS(
            std::chrono::duration_cast<std::chrono::milliseconds>(sleep_duration).count()));
}

} // namespace details

} // namespace this_task

} // namespace osal

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

void sleep_for(const std::chrono::milliseconds &sleep_duration)
{
    vTaskDelay(pdMS_TO_TICKS(sleep_duration.count()));
}

} // namespace details

} // namespace osal

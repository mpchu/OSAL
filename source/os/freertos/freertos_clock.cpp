#include "osal/os_clock.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

namespace osal
{

namespace chrono
{

using freertos_clock = details::os_clock<TickType_t, configTICK_RATE_HZ>;

freertos_clock::time_point freertos_clock::now() noexcept
{
    return freertos_clock::time_point(
            freertos_clock::duration(
                xTaskGetTickCount()));
}

} // namespace chrono

} // namespace osal

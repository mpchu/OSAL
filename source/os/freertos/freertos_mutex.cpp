#include "osal/os_mutex.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

using freertos_mutex = details::os_mutex<SemaphoreHandle_t>;

template<>
freertos_mutex::os_mutex()
{
    _handle = xSemaphoreCreateMutex();
    if (_handle == nullptr)
    {
        configASSERT(!"Mutex Constructor Failed");
    }
}

template<>
freertos_mutex::~os_mutex()
{
    vSemaphoreDelete(_handle);
}

template<>
void freertos_mutex::lock()
{
    xSemaphoreTake(_handle, portMAX_DELAY);
}

template<>
bool freertos_mutex::try_lock()
{
    BaseType_t success = xSemaphoreTake(_handle, 0);
    return (success == pdTRUE) ? true : false;
}

template<>
bool freertos_mutex::impl_try_lock_for(const std::chrono::nanoseconds &timeout)
{
    BaseType_t success = xSemaphoreTake(
        _handle,
        pdMS_TO_TICKS(
            std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()));
    return (success == pdTRUE) ? true : false;
}

template<>
void freertos_mutex::unlock()
{
    xSemaphoreGive(_handle);
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::freertos_mutex;

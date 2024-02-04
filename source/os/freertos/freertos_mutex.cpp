#include "osal/os_mutex.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

using freertos_mutex = details::os_mutex<SemaphoreHandle_t>;

template<>
freertos_mutex::os_mutex() : _handle(nullptr)
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
void freertos_mutex::unlock()
{
    xSemaphoreGive(_handle);
}

template<>
bool freertos_mutex::impl_try_lock_for(const osal::chrono::ticks &timeout)
{
    BaseType_t success = xSemaphoreTake(_handle, timeout.count());
    return (success == pdTRUE) ? true : false;
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::freertos_mutex;

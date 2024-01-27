#include "osal/os_mutex.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

template<>
mutex::os_mutex()
{
    _handle = xSemaphoreCreateMutex();
    if (_handle == nullptr)
    {
        configASSERT(!"Mutex Constructor Failed");
    }
}

template<>
mutex::~os_mutex()
{
    vSemaphoreDelete(_handle);
}

template<>
void mutex::lock()
{
    xSemaphoreTake(_handle, portMAX_DELAY);
}

template<>
bool mutex::try_lock()
{
    BaseType_t success = xSemaphoreTake(_handle, 0);
    return (success == pdTRUE) ? true : false;
}

template<>
bool mutex::try_lock_for(const std::chrono::milliseconds &timeout)
{
    BaseType_t success = xSemaphoreTake(_handle, pdMS_TO_TICKS(timeout.count()));
    return (success == pdTRUE) ? true : false;
}

template<>
void mutex::unlock()
{
    xSemaphoreGive(_handle);
}

}

// Instantiate the template specialization for this OS
template osal::mutex;
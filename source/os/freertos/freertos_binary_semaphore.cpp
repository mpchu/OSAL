#include "osal/os_binary_semaphore.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

using freertos_binary_semaphore = details::os_binary_semaphore<SemaphoreHandle_t>;

template<>
freertos_binary_semaphore::os_binary_semaphore(bool desired) : _handle(nullptr)
{
    _handle = xSemaphoreCreateBinary();
    if (_handle == nullptr)
    {
        configASSERT(!"Binary Semaphore Constructor Failed");
    }

    if (desired)
    {
        release();
    }
}

template<>
freertos_binary_semaphore::~os_binary_semaphore()
{
    vSemaphoreDelete(_handle);
}

template<>
void freertos_binary_semaphore::release()
{
    xSemaphoreGive(_handle);
}

template<>
void freertos_binary_semaphore::acquire()
{
    xSemaphoreTake(_handle, portMAX_DELAY);
}

template<>
bool freertos_binary_semaphore::try_acquire()
{
    BaseType_t success = xSemaphoreTake(_handle, 0);
    return (success == pdTRUE) ? true : false;
}

template<>
bool freertos_binary_semaphore::impl_try_acquire_for(const osal::chrono::ticks &timeout)
{
    BaseType_t success = xSemaphoreTake(_handle, timeout.count());
    return (success == pdTRUE) ? true : false;
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::freertos_binary_semaphore;

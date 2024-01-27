#include "osal/os_binary_semaphore.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

template<>
binary_semaphore::os_binary_semaphore(bool desired)
{
    _handle = xSemaphoreCreateBinary();
    if (_handle == nullptr)
    {
        configASSERT(!"Binary Semaphore Constructor Failed");
    }

    if (!desired)
    {
        release();
    }
}

template<>
binary_semaphore::~os_binary_semaphore()
{
    vSemaphoreDelete(_handle);
}

template<>
void binary_semaphore::release()
{
    xSemaphoreGive(_handle);
}

template<>
void binary_semaphore::acquire()
{
    xSemaphoreTake(_handle, portMAX_DELAY);
}

template<>
bool binary_semaphore::try_acquire()
{
    BaseType_t success = xSemaphoreTake(_handle, 0);
    return (success == pdTRUE) ? true : false;
}

template<>
bool binary_semaphore::try_acquire_for(const std::chrono::milliseconds &timeout)
{
    BaseType_t success = xSemaphoreTake(_handle, timeout.count());
    return (success == pdTRUE) ? true : false;
}

}

// Instantiate the template specialization for this OS
template osal::binary_semaphore;
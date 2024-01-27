#include "osal/os_counting_semaphore.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

template<>
counting_semaphore::os_counting_semaphore(std::size_t max_count, std::size_t initial)
{
    _handle = xSemaphoreCreateBinary();
    if (_handle == nullptr)
    {
        configASSERT(!"Binary Semaphore Constructor Failed");
    }
}

template<>
counting_semaphore::~os_counting_semaphore()
{
    vSemaphoreDelete(_handle);
}

template<>
void counting_semaphore::release(std::size_t update)
{
    for (std::size_t i = 0; i < update; ++i)
    {
        xSemaphoreGive(_handle);
    }
}

template<>
void counting_semaphore::acquire()
{
    xSemaphoreTake(_handle, portMAX_DELAY);
}

template<>
bool counting_semaphore::try_acquire()
{
    BaseType_t success = xSemaphoreTake(_handle, 0);
    return (success == pdTRUE) ? true : false;
}

template<>
bool counting_semaphore::try_acquire_for(const std::chrono::milliseconds &timeout)
{
    BaseType_t success = xSemaphoreTake(_handle, timeout.count());
    return (success == pdTRUE) ? true : false;
}

}

// Instantiate the template specialization for this OS
template osal::counting_semaphore;
#include "osal/os_counting_semaphore.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace osal
{

using freertos_counting_semaphore = details::os_counting_semaphore<SemaphoreHandle_t>;

template<>
freertos_counting_semaphore::os_counting_semaphore(std::size_t max_count, std::size_t initial)
    : _handle(nullptr),
      _max_count(max_count)
{
    _handle = xSemaphoreCreateCounting(_max_count, initial);
    if (_handle == nullptr)
    {
        configASSERT(!"Binary Semaphore Constructor Failed");
    }
}

template<>
freertos_counting_semaphore::~os_counting_semaphore()
{
    vSemaphoreDelete(_handle);
}

template<>
void freertos_counting_semaphore::release(std::size_t update)
{
    for (std::size_t i = 0; i < update; ++i)
    {
        xSemaphoreGive(_handle);
    }
}

template<>
void freertos_counting_semaphore::acquire()
{
    xSemaphoreTake(_handle, portMAX_DELAY);
}

template<>
bool freertos_counting_semaphore::try_acquire()
{
    BaseType_t success = xSemaphoreTake(_handle, 0);
    return (success == pdTRUE) ? true : false;
}

template<>
bool freertos_counting_semaphore::impl_try_acquire_for(const std::chrono::nanoseconds &timeout)
{
    BaseType_t success = xSemaphoreTake(
        _handle, 
        pdMS_TO_TICKS(
            std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()));
    return (success == pdTRUE) ? true : false;
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::freertos_counting_semaphore;

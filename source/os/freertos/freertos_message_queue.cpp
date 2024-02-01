#include "osal/os_message_queue.h"
#include <cstring>
#include "FreeRTOS.h"
#include "queue.h"

namespace osal
{

using freertos_message_queue = details::os_message_queue<QueueHandle_t>;

template<>
freertos_message_queue::os_message_queue(osal::string_view name, std::size_t item_size, std::size_t depth)
    : _item_size(item_size),
      _max_num_items(depth)
{
    std::memset(_name, '\0', sizeof(_name));
    std::strncpy(_name, name, sizeof(_name) - 1);

    _handle = xQueueCreate(depth, item_size);
    if (_handle == nullptr)
    {
        configASSERT(!"Queue Constructor Failed");
    }
}

template<>
freertos_message_queue::~os_message_queue()
{
    vQueueDelete(_handle);
}

template<>
int freertos_message_queue::impl_send(const void *data,
                                      std::size_t num_bytes,
                                      const std::chrono::nanoseconds &timeout)
{
    TickType_t ticks = (timeout == infinite_timeout)
                       ? portMAX_DELAY
                       : pdMS_TO_TICKS(
                           std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count());
    BaseType_t rtosCode = xQueueSendToBack(_handle, data, ticks);
    return (rtosCode == pdTRUE) ? 0 : -1;
}

template<>
int freertos_message_queue::impl_receive(void *buffer,
                                         std::size_t buffer_size,
                                         const std::chrono::nanoseconds &timeout)
{
    int status = 0;
    if (buffer_size < _item_size)
    {
        status = -1;
    }
    else
    {
        TickType_t ticks = (timeout == infinite_timeout)
                           ? portMAX_DELAY
                           : pdMS_TO_TICKS(
                               std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count());
        BaseType_t rtosCode = xQueueReceive(_handle, buffer, ticks);
        if (rtosCode != pdTRUE)
        {
            status = -2;
        }
    }
    return status;
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::freertos_message_queue;

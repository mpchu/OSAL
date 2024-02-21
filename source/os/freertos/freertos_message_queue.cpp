#include "osal/os_message_queue.h"
#include <cstring>
#include "FreeRTOS.h"
#include "queue.h"

namespace osal
{

using freertos_message_queue = details::os_message_queue<QueueHandle_t>;

template<>
freertos_message_queue::os_message_queue(const char *name, std::size_t depth, std::size_t item_size)
    : _handle(nullptr),
      _max_num_items(depth),
      _item_size(item_size)
{
    std::memset(_name, '\0', sizeof(_name));
    std::strncpy(_name, name, sizeof(_name) - 1);

    _handle = xQueueCreate(_max_num_items, _item_size);
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
mq_status freertos_message_queue::impl_send(const void *data,
                                            std::size_t num_bytes,
                                            const osal::chrono::ticks &timeout)
{
    mq_status status = mq_status::success;
    if (data == nullptr)
    {
        status = mq_status::null_buffer;
    }
    else
    {
        TickType_t ticks = (timeout == infinite_timeout)
                           ? portMAX_DELAY
                           : timeout.count();
        BaseType_t rtosCode = xQueueSendToBack(_handle, data, ticks);
        if (rtosCode != pdTRUE)
        {
            status = mq_status::timeout;
        }
    }
    return status;
}

template<>
mq_status freertos_message_queue::impl_receive(void *buffer,
                                               std::size_t buffer_size,
                                               const osal::chrono::ticks &timeout)
{
    mq_status status = mq_status::success;
    if (buffer == nullptr)
    {
        status = mq_status::null_buffer;
    }
    else if (buffer_size < _item_size)
    {
        status = mq_status::invalid_length;
    }
    else
    {
        TickType_t ticks = (timeout == infinite_timeout)
                           ? portMAX_DELAY
                           : timeout.count();
        BaseType_t rtosCode = xQueueReceive(_handle, buffer, ticks);
        if (rtosCode != pdTRUE)
        {
            status = mq_status::timeout;
        }
    }
    return status;
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::freertos_message_queue;

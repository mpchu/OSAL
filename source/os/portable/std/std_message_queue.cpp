#include "osal/os_message_queue.h"
#include "os/portable/std/internal/std_queue_handle.h"


namespace osal
{

using std_message_queue = details::os_message_queue<details::std_queue_handle *>;

template<>
std_message_queue::os_message_queue(const char *name, std::size_t depth, std::size_t item_size)
    : _handle(nullptr),
      _max_num_items(depth),
      _item_size(item_size)
{
    std::memset(_name, '\0', sizeof(_name));
    std::strncpy(_name, name, sizeof(_name) - 1);

    _handle = new details::std_queue_handle(_max_num_items, _item_size);
}

template<>
std_message_queue::~os_message_queue()
{
    delete _handle;
}

template<>
int std_message_queue::impl_send(const void *data,
                                 std::size_t num_bytes,
                                 const osal::chrono::ticks &timeout)
{
    int status = 0;
    if (timeout == infinite_timeout)
    {
        status = _handle->impl_send(
            reinterpret_cast<const uint8_t *>(data),
            num_bytes);
    }
    else
    {
        status = _handle->impl_try_send_for(
            reinterpret_cast<const uint8_t *>(data),
            num_bytes,
            timeout);
    }
    return status;
}

template<>
int std_message_queue::impl_receive(void *buffer,
                                    std::size_t buffer_size,
                                    const osal::chrono::ticks &timeout)
{
    int status = 0;
    if (timeout == infinite_timeout)
    {
        status = _handle->impl_receive(
            reinterpret_cast<uint8_t *>(buffer),
            buffer_size);
    }
    else
    {
        status = _handle->impl_try_receive_for(
            reinterpret_cast<uint8_t *>(buffer),
            buffer_size,
            timeout);
    }
    return status;
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::std_message_queue;

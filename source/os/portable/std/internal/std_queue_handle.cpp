#include "os/portable/std/internal/std_queue_handle.h"
#include <cstring>

namespace osal
{

namespace details
{

std_queue_handle::std_queue_handle(std::size_t item_size, std::size_t depth) : _item_size(item_size), _depth(depth)
{
}

int std_queue_handle::impl_send(const uint8_t *data, std::size_t num_bytes)
{
    int status = 0;
    if (data == nullptr)
    {
        status = -1;
    }
    else if (_item_size < num_bytes)
    {
        status = -2;
    }
    else
    {
        auto queueItem = std::vector<uint8_t>(_item_size);
        std::memcpy(queueItem.data(), data, num_bytes);
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condvar.wait(lock, [this] { return _queue.size() < _depth; });

            _queue.push(std::move(queueItem));
            _condvar.notify_one();
        }
    }
    return status;
}

int std_queue_handle::impl_receive(uint8_t *buffer, std::size_t buffer_size)
{
    int status = 0;
    if (buffer == nullptr)
    {
        status = -1;
    }
    else if (_item_size > buffer_size)
    {
        status = -2;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condvar.wait(lock, [this] { return _queue.size() > 0; });

        std::memcpy(buffer, _queue.front().data(), _item_size);
        _queue.pop();
        _condvar.notify_one();
    }
    return status;
}

int std_queue_handle::impl_try_send(const uint8_t *data, std::size_t num_bytes)
{
    int status = 0;
    if (data == nullptr)
    {
        status = -1;
    }
    else if (_item_size < num_bytes)
    {
        status = -2;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.size() < _depth)
        {
            auto queueItem = std::vector<uint8_t>(_item_size);
            std::memcpy(queueItem.data(), data, num_bytes);
            _queue.push(std::move(queueItem));
            _condvar.notify_one();
        }
        else
        {
            status = -3;
        }
    }
    return status;
}

int std_queue_handle::impl_try_receive(uint8_t *buffer, std::size_t buffer_size)
{
    int status = 0;
    if (buffer == nullptr)
    {
        status = -1;
    }
    else if (_item_size > buffer_size)
    {
        status = -2;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.size())
        {
            std::memcpy(buffer, _queue.front().data(), _item_size);
            _queue.pop();
            _condvar.notify_one();
        }
        else
        {
            status = -3;
        }
    }
    return status;
}

int std_queue_handle::impl_try_send_for(const uint8_t *data,
                                        std::size_t num_bytes,
                                        const std::chrono::nanoseconds &timeout)
{
    int status = 0;
    if (data == nullptr)
    {
        status = -1;
    }
    else if (_item_size < num_bytes)
    {
        status = -2;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        bool success = _condvar.wait_for(lock, timeout, [this] { return _queue.size() < _depth; });
        if (!success)
        {
            status = -3;
        }
        else
        {
            auto queueItem = std::vector<uint8_t>(_item_size);
            std::memcpy(queueItem.data(), data, num_bytes);
            _queue.push(std::move(queueItem));
            _condvar.notify_one();
        }
    }
    return status;
}

int std_queue_handle::impl_try_receive_for(uint8_t *buffer,
                                           std::size_t buffer_size,
                                           const std::chrono::nanoseconds &timeout)
{
    int status = 0;
    if (buffer == nullptr)
    {
        status = -1;
    }
    else if (_item_size > buffer_size)
    {
        status = -2;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        bool success = _condvar.wait_for(lock, timeout, [this] { return _queue.size() > 0; });
        if (!success)
        {
            status = -3;
        }
        else
        {
            std::memcpy(buffer, _queue.front().data(), _item_size);
            _queue.pop();
            _condvar.notify_one();
        }
    }
    return status;
}


}

}

#ifndef _OSAL_SOURCE_OS_PORTABLE_STD_INTERNAL_STD_QUEUE_HANDLE_H_
#define _OSAL_SOURCE_OS_PORTABLE_STD_INTERNAL_STD_QUEUE_HANDLE_H_

#include <condition_variable>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>

namespace osal
{

namespace details
{

class std_queue_handle
{
    const std::size_t _item_size;

    const std::size_t _depth;

    std::queue<std::unique_ptr<uint8_t>> _queue;

    std::mutex _mutex;

    std::condition_variable _condvar;

public:
    std_queue_handle(std::size_t item_size, std::size_t depth);

    ~std_queue_handle() = default;

    int impl_send(const uint8_t *data, std::size_t num_bytes);

    int impl_receive(uint8_t *buffer, std::size_t buffer_size);

    int impl_try_send(const uint8_t *data, std::size_t num_bytes);

    int impl_try_receive(uint8_t *buffer, std::size_t buffer_size);

    int impl_try_send_for(const uint8_t *data,
                          std::size_t num_bytes,
                          const std::chrono::nanoseconds &timeout);

    int impl_try_receive_for(uint8_t *buffer,
                             std::size_t buffer_size,
                             const std::chrono::nanoseconds &timeout);
};

}

}

#endif
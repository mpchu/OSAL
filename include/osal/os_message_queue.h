#ifndef _OSAL_OS_MESSAGE_QUEUE_H_
#define _OSAL_OS_MESSAGE_QUEUE_H_

#include <chrono>
#include <cstddef>
#include "osal/osal_config.h"

namespace osal
{

namespace api
{

/**
 * @brief Portable IPC message queue object.
 * This class template declares operations and attributes common to all operating system message queues.
 * @tparam handle_t Native OS handle type for IPC message queues
 */
template <typename handle_t = configOSAL_MSG_QUEUE_NATIVE_HANDLE>
class os_message_queue
{
    char _name[configOSAL_MAXIMUM_QUEUE_NAME_SIZE];

    handle_t _handle;

    std::size_t _item_size;

    std::size_t _max_num_items;

public:
    using native_handle_type = handle_t; /**< Underlying OS queue handle implementation */

    /**
     * @brief Creates a new IPC message queue in the operating system.
     * The constructor definition will perform operating system specific operations to create a new queue.
     * @param[in] name The name of the new queue being created
     * @param[in] item_size Size of a single item in the queue
     * @param[in] depth The maximum number of items that can be stored in the queue
     */
    os_message_queue(osal::string_view name, std::size_t item_size, std::size_t depth);

    /**
     * @brief Destroys the IPC message queue and cleans up any OS resources that were in use.
     */
    ~os_message_queue();

    /**
     * @brief Returns the underlying operating system queue handle for this message queue.
     * @return Native OS queue handle
     */
    native_handle_type native_handle() { return _handle; }

    /**
     * @brief Sends data through the message queue.
     * @param[in] data Buffer containing data to be sent
     * @param[in] num_bytes Number of bytes to send
     * @param[in] timeout Maximum amount of time to attempt sending data
     * @return 0 on success, error codes otherwise
     */
    int send(const void *data,
             std::size_t num_bytes,
             const std::chrono::milliseconds &timeout = infinite_timeout);

    /**
     * @brief Receives data from the message queue.
     * @param[out] buffer Buffer to store data received from the message queue
     * @param[in] buffer_size Size of receive buffer in bytes
     * @param[in] timeout Maximum amount of time to attempt receiving data
     * @return 0 on success, error codes otherwise 
     */
    int receive(void *buffer,
                std::size_t buffer_size,
                const std::chrono::milliseconds &timeout = infinite_timeout);

    /**
     * @brief Attempts to send data through the message queue without blocking.
     * @param[in] data Buffer containing data to be sent
     * @param[in] num_bytes Number of bytes to send
     * @return True on successful send, false otherwise
     */
    bool try_send(const void *data, std::size_t num_bytes);

    /**
     * @brief Attempts to receive data from the message queue without blocking.
     * @param[in] buffer Buffer to store data received from the message queue
     * @param[in] buffer_size Size of receive buffer in bytes
     * @return True on successful receive, false otherwise
     */
    bool try_receive(void *buffer, std::size_t buffer_size);

    /**
     * @brief Returns the size of each item in the queue.
     * @return Size of each item in the queue
     */
    std::size_t item_size() { return _item_size; }

    /**
     * @brief Returns the maximum number of items that can be stored in the queue.
     * @return Maximum number of items that can be stored in the queue
     */
    std::size_t depth() { return _max_num_items; }

    /**
     * @brief Returns the name of the IPC message queue.
     * @return Name of the IPC message queue
     */
    osal::string_view name() { return _name; }

    static constexpr std::chrono::milliseconds infinite_timeout = std::chrono::milliseconds(-1); /**< Timeout value used to block indefinitely on message queue operations */
};

} // namespace api

using message_queue = api::os_message_queue<>;

} // namespace osal

#endif

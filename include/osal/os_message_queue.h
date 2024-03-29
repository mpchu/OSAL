#ifndef _OSAL_INCLUDE_OSAL_OS_MESSAGE_QUEUE_H_
#define _OSAL_INCLUDE_OSAL_OS_MESSAGE_QUEUE_H_

#include <chrono>
#include <cstddef>
#include "osal/osal_config.h"
#include "osal/os_clock.h"

namespace osal
{

/**
 * @enum mq_status
 * Enumeration describing whether a message queue operation
 * returned due to success or some error.
 */
enum class mq_status : int
{
    success = 0,
    null_buffer = -1,
    invalid_length = -2,
    timeout = -3
};

namespace details
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

    std::size_t _max_num_items;

    std::size_t _item_size;

public:
    using native_handle_type = handle_t; /**< Underlying OS queue handle implementation */

    /**
     * @brief Creates a new IPC message queue in the operating system.
     * The constructor definition will perform operating system specific operations to create a new queue.
     * @param[in] name The name of the new queue being created
     * @param[in] depth The maximum number of items that can be stored in the queue
     * @param[in] item_size Size of a single item in the queue
     */
    os_message_queue(const char *name, std::size_t depth, std::size_t item_size);

    /**
     * @brief Destroys the IPC message queue and cleans up any OS resources that were in use.
     */
    ~os_message_queue();

    /**
     * @brief Returns the underlying operating system queue handle for this message queue.
     * @return Native OS queue handle
     */
    native_handle_type native_handle() const { return _handle; }

    /**
     * @brief Sends data through the message queue. If the message queue is full,
     *        then this method indefinitely blocks until it can send.
     * @param[in] data Buffer containing data to be sent
     * @param[in] num_bytes Number of bytes to send
     * @return 0 on success, error codes otherwise
     */
    mq_status send(const void *data, std::size_t num_bytes)
    {
        return impl_send(data, num_bytes, infinite_timeout);
    }

    /**
     * @brief Receives data from the message queue. If the message queue does not have data,
     *        then this method indefinitely blocks until there is data to receive.
     * @param[out] buffer Buffer to store data received from the message queue
     * @param[in] buffer_size Size of receive buffer in bytes
     * @return 0 on success, error codes otherwise 
     */
    mq_status receive(void *buffer, std::size_t buffer_size)
    {
        return impl_receive(buffer, buffer_size, infinite_timeout);
    }

    /**
     * @brief Sends data through the message queue. Returns if unable to send data through
     *        the message queue for the specified timeout duration.
     * @param[in] data Buffer containing data to be sent
     * @param[in] num_bytes Number of bytes to send
     * @param[in] timeout Maximum amount of time to attempt sending data
     * @return 0 on success, error codes otherwise
     */
    template <class Rep, class Period>
    mq_status send(const void *data,
                   std::size_t num_bytes,
                   const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_send(
            data,
            num_bytes,
            std::chrono::duration_cast<osal::chrono::ticks>(timeout));
    }

    /**
     * @brief Receives data from the message queue. Returns if unable to receive data from
     *        the message queue for the specified timeout duration.
     * @param[out] buffer Buffer to store data received from the message queue
     * @param[in] buffer_size Size of receive buffer in bytes
     * @param[in] timeout Maximum amount of time to attempt receiving data
     * @return 0 on success, error codes otherwise 
     */
    template <class Rep, class Period>
    mq_status receive(void *buffer,
                      std::size_t buffer_size,
                      const std::chrono::duration<Rep, Period> &timeout)
    {
        return impl_receive(
            buffer,
            buffer_size,
            std::chrono::duration_cast<osal::chrono::ticks>(timeout));
    }

    /**
     * @brief Attempts to send data through the message queue without blocking.
     * @param[in] data Buffer containing data to be sent
     * @param[in] num_bytes Number of bytes to send
     * @return True on successful send, false otherwise
     */
    bool try_send(const void *data, std::size_t num_bytes)
    {
        return impl_send(data, num_bytes, osal::chrono::ticks(0)) == mq_status::success;
    }

    /**
     * @brief Attempts to receive data from the message queue without blocking.
     * @param[in] buffer Buffer to store data received from the message queue
     * @param[in] buffer_size Size of receive buffer in bytes
     * @return True on successful receive, false otherwise
     */
    bool try_receive(void *buffer, std::size_t buffer_size)
    {
        return impl_receive(buffer, buffer_size, osal::chrono::ticks(0)) == mq_status::success;
    }

    /**
     * @brief Returns the size of each item in the queue.
     * @return Size of each item in the queue in bytes
     */
    std::size_t item_size() const { return _item_size; }

    /**
     * @brief Returns the maximum number of items that can be stored in the queue.
     * @return Maximum number of items that can be stored in the queue
     */
    std::size_t depth() const { return _max_num_items; }

    /**
     * @brief Returns the name of the IPC message queue.
     * @return Name of the IPC message queue
     */
    const char *name() const { return _name; }

private:
    // Delete copy & move
    os_message_queue(const os_message_queue &rhs) = delete;
    os_message_queue(os_message_queue &&rhs) = delete;

    static constexpr osal::chrono::ticks infinite_timeout = osal::chrono::ticks(-1); /**< Timeout value used to block indefinitely on message queue operations */

    /**
     * @brief OS-specific implementation for sending data through the message queue.
     * @param[in] data Buffer containing data to be sent
     * @param[in] num_bytes Number of bytes to send
     * @param[in] timeout Maximum amount of time to attempt sending data, if -1 then blocks indefinitely
     * @return 0 on success, error codes otherwise 
     */
    mq_status impl_send(const void *data,
                        std::size_t num_bytes,
                        const osal::chrono::ticks &timeout = infinite_timeout);

    /**
     * @brief OS-specific implementation for receiving data from the message queue.
     * @param[out] buffer Buffer to store data received from the message queue
     * @param[in] buffer_size Size of receive buffer in bytes
     * @param[in] timeout Maximum amount of time to attempt receiving data, if -1 then blocks indefinitely
     * @return 0 on success, error codes otherwise 
     */
    mq_status impl_receive(void *buffer,
                           std::size_t buffer_size,
                           const osal::chrono::ticks &timeout = infinite_timeout);
};

} // namespace details

using message_queue = details::os_message_queue<>;

} // namespace osal

#endif

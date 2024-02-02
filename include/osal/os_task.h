#ifndef _OSAL_INCLUDE_OSAL_OS_TASK_H_
#define _OSAL_INCLUDE_OSAL_OS_TASK_H_

#include <cstddef>
#include <cstring>
#include <functional>
#include <type_traits>
#include "osal/osal_config.h"
#include "osal/os_task_attributes.h"

namespace osal
{

namespace details
{

/**
 * @brief Public API for an operating system task object.
 * This class template declares operations and attributes common to all operating system tasks.
 * @tparam handle_t OS-specific task handle
 */
template <class handle_t = configOSAL_TASK_NATIVE_HANDLE>
class os_task
{
    using callable_task_fn = std::function<void()>;

    task_attributes _attributes;

    callable_task_fn _task_def;

    handle_t _handle;

public:
    using native_handle_type = handle_t; /**< Underlying OS task handle implementation */

    /**
     * @brief Creates a new task in the operating system and executes it.
     * @tparam Function Function to be executed by the task
     * @param[in] attr Task attributes
     * @param[in] func Function to execute
     */
    template <class Function>
    explicit os_task(task_attributes attr, Function &&func)
        : _attributes(std::forward<task_attributes>(attr)),
          _task_def(std::forward<Function>(func))
    {
        create_task();
    }

    /**
     * @brief Creates a new task in the operating system and executes it.
     * @tparam Function Function to be executed by the task
     * @tparam Args Parameter pack of arguments to be passed to Function
     * @param[in] attr Task attributes
     * @param[in] func Function to execute
     * @param[in] args Arguments to be passed to func
     */
    template <class Function, class... Args>
    explicit os_task(task_attributes attr, Function &&func, Args &&... args)
        : _attributes(std::forward<task_attributes>(attr)),
          _task_def(create_callable_task(std::forward<Function>(func), std::forward<Args>(args)...))
    {
        create_task();
    }

    /**
     * @brief Destroys the task and cleans up any OS resources that were in use.
     */
    ~os_task()
    {
        destroy_task();
    }

    os_task(const os_task &rhs) = delete;

    os_task(os_task &&rhs) = default;

    /**
     * @brief Returns the underlying operating system task handle for this object.
     * @return Native OS task handle
     */
    native_handle_type native_handle() { return _handle; }

    /**
     * @brief Returns the task attributes.
     * @return Task attributes 
     */
    task_attributes attributes() { return _attributes; }

private:
    /**
     * @brief Binds a parameter pack to a function to create a callable task definition object.
     * @tparam Function Function to be executed by the task
     * @tparam Args Parameter pack of arguments to be passed to Function
     * @param[in] func Function to execute
     * @param[in] args Arguments to be passed to func
     * @return Callable task definition function
     */
    template <class Function, class... Args>
    callable_task_fn create_callable_task(Function &&func, Args &&... args)
    {
#if __cplusplus >= 201703L
        return [fn = std::forward<Function>(func), args = std::make_tuple(std::forward<Args>(args)...)]()
        {
            std::apply(fn, args);
        };
#else
        // If not using C++17 then use std::bind() in place of std::apply()
        /// @note NOTE: This implementation has not been thoroughly tested
        /// and there can be many potential issues when creating tasks caused by this
        return std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
#endif
    }

    template <class Return, class... Args>
    static Return task_entry_point(Args ... args);

    template <class OsPriority>
    static OsPriority translate_priority(int priority);

    void create_task();

    void destroy_task();
};

} // namespace details

using task = details::os_task<>;

} // namespace osal

#endif
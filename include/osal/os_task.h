#ifndef _OSAL_OS_TASK_H_
#define _OSAL_OS_TASK_H_

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

template <class task_impl = configOSAL_TASK_IMPLEMENTATION>
class os_task
{
    task_impl _implementation;

    using callable_task_fn = std::function<void()>;

public:
    using native_handle_type = configOSAL_TASK_NATIVE_HANDLE;

    template<class Function>
    explicit os_task(task_attributes attr, Function &&func)
        : _implementation(std::forward<task_attributes>(attr),
                          std::forward<Function>(func))
    {
    }

    template<class Function, class... Args>
    explicit os_task(task_attributes attr, Function &&func, Args &&... args)
        : _implementation(std::forward<task_attributes>(attr),
                          create_callable_task(std::forward<Function>(func), std::forward<Args>(args)...))
    {
    }

    ~os_task() = default;

    os_task(const os_task &rhs) = delete;

    os_task(os_task &&rhs) = default;

    native_handle_type native_handle() { return _implementation.native_handle(); }

    task_attributes attributes() { return _implementation.attributes(); }

private:
    template<class Function, class... Args>
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

};

} // namespace details

using task = details::os_task<>;

} // namespace osal

#endif
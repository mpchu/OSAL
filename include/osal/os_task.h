#ifndef _OSAL_OS_TASK_H_
#define _OSAL_OS_TASK_H_

#include <cstddef>
#include <cstring>
#include "osal/osal_config.h"
#include "osal/os_task_attributes.h"

namespace osal
{

namespace api
{

template <class task_impl = configOSAL_TASK_IMPLEMENTATION>
class os_task
{
    task_impl _implementation;

public:
    using native_handle_type = configOSAL_TASK_NATIVE_HANDLE;

    template<class Function, class... Args>
    explicit os_task(task_attributes attr, Function &&func, Args &&... args)
        : _implementation(std::forward<task_attributes>(attr), std::forward<Function>(func), std::forward<Args>(args)...)
    {
    }

    ~os_task() = default;

    os_task(const os_task &rhs) = delete;

    os_task(os_task &&rhs) = default;

    native_handle_type native_handle() { return _implementation.native_handle(); }

    task_attributes attributes() { return _implementation.attributes(); }
};

} // namespace api

using task = api::os_task<>;

} // namespace osal

#endif
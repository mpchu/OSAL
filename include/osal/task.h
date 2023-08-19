#ifndef _OSAL_TASK_H_
#define _OSAL_TASK_H_

#include <cstddef>
#include <cstring>
#include "osal/osal_default_config.h"

namespace osal
{

template <typename handle_t>
class task
{
    handle_t _task_handle;

    task::attributes _attributes;

public:
    using native_handle_type = handle_t;

    struct attributes
    {
    private:
        char _name[configOSAL_MAXIMUM_TASK_NAME_SIZE];
        std::size_t _stack_size;
        int _priority;

    public:
        attributes &set_name(osal::string_view name)  { std::memset(_name, '\0', sizeof(_name)); std::strncpy(_name, name, sizeof(_name) - 1); return *this; }
        attributes &set_stack_size(std::size_t bytes) { _stack_size = bytes; return *this; }
        attributes &set_priority(int priority)        { _priority = priority; return *this; }

        osal::string_view name() { return _name; }
        std::size_t stack_size() { return _stack_size; }
        int priority()           { return _priority; }
    };

    template<class Function, class... Args>
    explicit task(osal::task::attributes attr, Function &&func, Args &&... args);

    ~task();

    task(const osal::task &rhs) = delete;

    task(osal::task &&rhs) noexcept;

    native_handle_type native_handle() { return _task_handle; }

    task::attributes attributes() { return _attributes; }

};

}

#endif
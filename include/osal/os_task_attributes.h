#ifndef _OSAL_INCLUDE_OSAL_TASK_ATTRIBUTES_H_
#define _OSAL_INCLUDE_OSAL_TASK_ATTRIBUTES_H_

#include <cstddef>
#include <cstring>
#include "osal/osal_config.h"

namespace osal
{

struct task_attributes
{
private:
    char _name[configOSAL_MAXIMUM_TASK_NAME_SIZE];
    uint8_t *_stack_ptr;
    std::size_t _stack_size;
    int _priority;

public:
    task_attributes() : _stack_ptr(nullptr), _stack_size(0), _priority(0) { std::memset(_name, '\0', sizeof(_name)); }
    ~task_attributes() = default;

    task_attributes &set_name(const char *name)
    {
        std::memset(_name, '\0', sizeof(_name));
        std::strncpy(_name, name, sizeof(_name) - 1);
        return *this;
    }

    task_attributes &set_stack_pointer(uint8_t *stack_ptr)
    {
        _stack_ptr = stack_ptr;
        return *this;
    }

    task_attributes &set_stack_size(std::size_t bytes)
    {
        _stack_size = bytes;
        return *this;
    }

    task_attributes &set_priority(int priority)
    {
        _priority = priority;
        return *this;
    }

    const char *name()       const { return _name; }
    uint8_t *stack_pointer() const { return _stack_ptr; }
    std::size_t stack_size() const { return _stack_size; }
    int priority()           const { return _priority; }
};

} // namespace osal

#endif
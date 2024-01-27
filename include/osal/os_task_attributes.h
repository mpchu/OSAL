#ifndef _OSAL_TASK_ATTRIBUTES_H_
#define _OSAL_TASK_ATTRIBUTES_H_

#include <cstdint>
#include "osal/osal_config.h"
#include "osal/osal_default_config.h"

namespace osal
{

struct task_attributes
{
private:
    char _name[configOSAL_MAXIMUM_TASK_NAME_SIZE];
    std::size_t _stack_size;
    int _priority;

public:
    task_attributes() : _stack_size(0), _priority(0) { std::memset(_name, '\0', sizeof(_name)); }
    ~task_attributes() = default;

    task_attributes &set_name(osal::string_view name)
    {
        std::memset(_name, '\0', sizeof(_name));
        std::strncpy(_name, name, sizeof(_name) - 1);
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

    osal::string_view name() { return _name; }
    std::size_t stack_size() { return _stack_size; }
    int priority()           { return _priority; }
};

} // namespace osal

#endif
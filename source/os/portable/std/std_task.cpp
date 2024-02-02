#include "osal/osal_config.h"
#include "osal/os_task.h"

#include <thread>

namespace osal
{

using std_task = details::os_task<std::thread *>;

template<>
template<>
void std_task::task_entry_point<void, void*>(void *taskObject)
{
    // Empty since unused in this implementation
}

template<>
template<>
int std_task::translate_priority<int>(int priority)
{
    // Just return the priority since unused in this implementation
    return priority;
}

template<>
void std_task::create_task()
{
    _handle = new std::thread(_task_def);
}

template<>
void std_task::destroy_task()
{
    delete _handle;
}

} // namespace osal

template osal::std_task;

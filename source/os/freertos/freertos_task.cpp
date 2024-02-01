#include "osal/osal_config.h"
#include "osal/os_task.h"

namespace osal
{

using freertos_task = details::os_task<TaskHandle_t>;

template<>
template<>
void freertos_task::task_entry_point<void, void*>(void *taskObject)
{
    freertos_task *myself = static_cast<freertos_task *>(taskObject);
    myself->_task_def();
}

template<>
template<>
UBaseType_t freertos_task::translate_priority<UBaseType_t>(int priority)
{
    // Translate from the public API priority to the actual RTOS priority
    constexpr BaseType_t numPriorities = configMAX_PRIORITIES;
    constexpr uint64_t priorityBucketSize = configOSAL_MAXIMUM_TASK_PRIORITY / numPriorities;
    UBaseType_t rtosPriority = priority / priorityBucketSize;
    return tskIDLE_PRIORITY + rtosPriority;
}

template<>
void freertos_task::create_task()
{
    BaseType_t success = xTaskCreate(&task_entry_point,
                                     _attributes.name(),
                                     _attributes.stack_size() / sizeof(configSTACK_DEPTH_TYPE),
                                     static_cast<void*>(this),
                                     translate_priority<UBaseType_t>(_attributes.priority()),
                                     &_handle);
    if (!success)
    {
        configASSERT(!"Task Constructor Failed");
    }
}

template<>
void freertos_task::destroy_task()
{
#if INCLUDE_vTaskDelayUntil == 1
    vTaskDelete(_handle);
#endif
}

} // namespace osal

template osal::freertos_task;

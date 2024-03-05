#include "osal/os_task.h"
#include "osal/osal_config.h"

#include <memory>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "task.h"

namespace osal
{

using freertos_task = details::os_task<TaskHandle_t, SemaphoreHandle_t>;

template<>
template<>
void freertos_task::task_entry_point<void, void*>(void *taskObject)
{
    freertos_task *myself = static_cast<freertos_task *>(taskObject);
    myself->run();
}

template<>
template<>
UBaseType_t freertos_task::translate_priority<UBaseType_t>(int priority)
{
    // Translate from the public API priority to the actual RTOS priority
    constexpr BaseType_t numPriorities = configMAX_PRIORITIES;
    constexpr uint64_t priorityBucketSize = configOSAL_NUM_TASK_PRIORITIES / numPriorities;
    UBaseType_t rtosPriority = priority / priorityBucketSize;
    return tskIDLE_PRIORITY + rtosPriority;
}

template<>
void freertos_task::create_task()
{
    BaseType_t success = pdFALSE;
    if (_attributes.stack_pointer() == nullptr)
    {
#if configSUPPORT_DYNAMIC_ALLOCATION == 1
        // Use the FreeRTOS heap implementation to dynamically allocate a stack
        success = xTaskCreate(&task_entry_point,
                              _attributes.name(),
                              _attributes.stack_size() / sizeof(configSTACK_DEPTH_TYPE),
                              static_cast<void*>(this),
                              translate_priority<UBaseType_t>(_attributes.priority()),
                              &_handle);
#else
        configASSERT(!"Dynamic task allocation is not enabled in the OS kernel");
#endif
    }
    else
    {
#if configSUPPORT_STATIC_ALLOCATION == 1
        // Align the application provided stack pointer with the data type used by FreeRTOS for task stacks
        std::size_t alignedStackSize = 0;
        void *alignedPointer = std::align(alignof(StackType_t),
                                          _attributes.stack_size(),
                                          _attributes.stack_pointer(),
                                          alignedStackSize);

        // Use the application provided stack memory to create the task
        StaticTask_t static_tcb = {0};
        success = xTaskCreateStatic(&task_entry_point,
                                    _attributes.name(),
                                    alignedStackSize / sizeof(configSTACK_DEPTH_TYPE),
                                    static_cast<void*>(this),
                                    translate_priority<UBaseType_t>(_attributes.priority()),
                                    reinterpret_cast<StackType_t *>(alignedPointer),
                                    &static_tcb); /// @todo TODO: Use the internal task record statically allocated TCB here
#else
        configASSERT(!"Static task allocation is not enabled in the OS kernel");
#endif
    }

    if (success == pdFALSE)
    {
        configASSERT(!"Task Constructor Failed");
    }
}

template<>
void freertos_task::destroy_task()
{
#if INCLUDE_vTaskDelete == 1
    vTaskDelete(_handle);
#else
    configASSERT(!"Task deletion is not enabled in the OS kernel");
#endif
}

} // namespace osal

template osal::freertos_task;

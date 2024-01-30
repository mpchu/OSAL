#include "os/freertos/freertos_task.h"
#include "osal/osal_config.h"

namespace osal
{

namespace details
{

/**
 * @brief Creates a new FreeRTOS task.
 * @param[in] attr Task Attributes
 * @param[in] task_definition Callable task definition
 */
freertos_task::freertos_task(task_attributes attr, std::function<void()> &&task_definition)
    : _attributes(attr),
      _handle(nullptr),
      _task_definition(std::forward<std::function<void()>>(task_definition))
{
    BaseType_t success = xTaskCreate(&task_entry_point,
                                     _attributes.name(),
                                     _attributes.stack_size() / sizeof(configSTACK_DEPTH_TYPE),
                                     static_cast<void*>(this),
                                     translate_priority(_attributes.priority()),
                                     &_handle);
    if (!success)
    {
        configASSERT(!"Task Constructor Failed");
    }
}

/**
 * @brief Destroys the freertos_task object and calls OS specific functions to delete the task.
 */
freertos_task::~freertos_task()
{
#if INCLUDE_vTaskDelayUntil == 1
    vTaskDelete(_handle);
#endif
}

/**
 * @brief Returns the underlying operating system task handle for this task object.
 * @return Native OS task handle
 */
freertos_task::native_handle_type freertos_task::native_handle()
{
    return _handle;
}

/**
 * @brief Returns the task attributes of this task object.
 * @return Task attributes 
 */
task_attributes freertos_task::attributes()
{
    return _attributes;
}

/**
 * @brief Begins execution of the task.
 */
void freertos_task::run()
{
    _task_definition();
}

/**
 * @brief Translates the priority passed in from the public API to an appropriate task priority used by the OS.
 * @param[in] priority Task priority ranging from 0 to configOSAL_MAXIMUM_TASK_PRIORITY
 * @return FreeRTOS task priority value
 */
UBaseType_t freertos_task::translate_priority(int priority)
{
    constexpr BaseType_t numPriorities = configMAX_PRIORITIES;
    constexpr uint64_t priorityBucketSize = configOSAL_MAXIMUM_TASK_PRIORITY / numPriorities;
    BaseType_t rtosPriority = priority / priorityBucketSize;
    return tskIDLE_PRIORITY + rtosPriority;
}

/**
 * @brief Task entry point function that FreeRTOS will use to run this specific task object.
 * @param task_object Pointer to the task being executed
 */
void freertos_task::task_entry_point(void *const task_object)
{
    freertos_task *myself = static_cast<freertos_task *>(task_object);
    myself->run();
}

} // namespace details

} // namespace osal

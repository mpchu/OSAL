#ifndef _OSAL_OS_FREERTOS_FREERTOS_TASK_H_
#define _OSAL_OS_FREERTOS_FREERTOS_TASK_H_

#include <functional>
#include <type_traits>

#include "osal/osal_config.h"
#include "osal/os_task_attributes.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

namespace osal
{

namespace details
{

class freertos_task
{
    task_attributes _attributes;

    TaskHandle_t _handle;

    std::function<void()> _task_definition;

public:
    using native_handle_type = TaskHandle_t;

    explicit freertos_task(task_attributes attr, std::function<void()> &&task_definition)
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

    ~freertos_task()
    {
#if INCLUDE_vTaskDelayUntil == 1
        vTaskDelete(_handle);
#endif
    }

    native_handle_type native_handle() { return _handle; }

    task_attributes attributes() { return _attributes; }

private:
    void run() { _task_definition(); }

    static UBaseType_t translate_priority(int priority)
    {
        constexpr BaseType_t numPriorities = configMAX_PRIORITIES;
        constexpr uint64_t priorityBucketSize = configOSAL_MAXIMUM_TASK_PRIORITY / numPriorities;
        BaseType_t rtosPriority = priority / priorityBucketSize;
        return tskIDLE_PRIORITY + rtosPriority;
    }

    static void task_entry_point(void *const task_object)
    {
        freertos_task *myself = static_cast<freertos_task *>(task_object);
        myself->run();
    }
};

}

}

#endif
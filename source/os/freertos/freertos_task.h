#ifndef _OSAL_OS_FREERTOS_FREERTOS_TASK_H_
#define _OSAL_OS_FREERTOS_FREERTOS_TASK_H_

#include <functional>

#include "osal/os_task_attributes.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

namespace osal
{

namespace details
{

/**
 * @brief Implementation class for a FreeRTOS task
 */
class freertos_task
{
    task_attributes _attributes;

    TaskHandle_t _handle;

    std::function<void()> _task_definition;

public:
    using native_handle_type = TaskHandle_t;

    explicit freertos_task(task_attributes attr, std::function<void()> &&task_definition);

    ~freertos_task();

    native_handle_type native_handle();

    task_attributes attributes();

private:
    void run();

    static UBaseType_t translate_priority(int priority);

    static void task_entry_point(void *const task_object);
};

} // namespace details

} // namespace osal

#endif
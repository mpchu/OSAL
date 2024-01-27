#ifndef _OSAL_CONFIG_H_
#define _OSAL_CONFIG_H_

#include <thread>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "os/freertos/freertos_task.h"

#define configOSAL_MUTEX_NATIVE_HANDLE     SemaphoreHandle_t
#define configOSAL_SEMAPHORE_NATIVE_HANDLE SemaphoreHandle_t
#define configOSAL_TASK_NATIVE_HANDLE      TaskHandle_t
#define configOSAL_MSG_QUEUE_NATIVE_HANDLE QueueHandle_t
#define configOSAL_TASK_IMPLEMENTATION     osal::details::freertos_task

#include "osal/osal_default_config.h"

#endif
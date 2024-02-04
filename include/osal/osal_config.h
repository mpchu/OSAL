#ifndef _OSAL_INCLUDE_OSAL_CONFIG_H_
#define _OSAL_INCLUDE_OSAL_CONFIG_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#define configOSAL_MUTEX_NATIVE_HANDLE     SemaphoreHandle_t
#define configOSAL_SEMAPHORE_NATIVE_HANDLE SemaphoreHandle_t
#define configOSAL_MSG_QUEUE_NATIVE_HANDLE QueueHandle_t
#define configOSAL_TASK_NATIVE_HANDLE      TaskHandle_t

#define configOSAL_TICK_TYPE               TickType_t
#define configOSAL_TICK_RATE_HZ            configTICK_RATE_HZ

#include "osal/osal_default_config.h"

#endif
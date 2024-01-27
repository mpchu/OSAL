#ifndef _OSAL_OS_THIS_TASK_H_
#define _OSAL_OS_THIS_TASK_H_

#include <chrono>

namespace osal
{

namespace this_task
{

void yield() noexcept;

void sleep_for(const std::chrono::milliseconds &sleep_duration);

}

}

#endif
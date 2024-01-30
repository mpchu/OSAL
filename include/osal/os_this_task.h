#ifndef _OSAL_OS_THIS_TASK_H_
#define _OSAL_OS_THIS_TASK_H_

#include <chrono>

namespace osal
{

namespace this_task
{

/**
 * @brief Provides a hint to the operating system scheduler to yield execution time for this task to other tasks.
 */
void yield() noexcept;

/**
 * @brief Stops execution of this task for a set period of time.
 * @param sleep_duration Time to sleep for in milliseconds
 */
void sleep_for(const std::chrono::milliseconds &sleep_duration);

} // namespace this_task

} // namespace osal

#endif
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
 * @tparam Rep Arithmetic type representing the units
 * @tparam Period A rational fraction representing the unit period in seconds
 * @param[in] sleep_duration Duration to sleep for
 */
template <class Rep, class Period>
void sleep_for(const std::chrono::duration<Rep, Period> &sleep_duration)
{
    details::os_sleep_for(std::chrono::duration_cast<std::chrono::nanoseconds>(sleep_duration));
}

namespace details
{

void os_sleep_for(const std::chrono::nanoseconds &sleep_duration);

} // namespace details

} // namespace this_task

} // namespace osal

#endif
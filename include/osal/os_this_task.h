#ifndef _OSAL_INCLUDE_OSAL_OS_THIS_TASK_H_
#define _OSAL_INCLUDE_OSAL_OS_THIS_TASK_H_

#include <chrono>
#include "osal/os_clock.h"

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
    details::os_sleep_for(std::chrono::duration_cast<osal::chrono::ticks>(sleep_duration));
}

/**
 * @brief Stops execution of this task until the specified point in time has been reached.
 * @tparam Clock The clock on which the time point is measured
 * @param[in] wakeup_time Time to block until
 */
template <class Clock, class Duration>
void sleep_until(const std::chrono::time_point<Clock, Duration> &wakeup_time)
{
    details::os_sleep_until(std::chrono::time_point_cast<Clock::duration>(wakeup_time));
}

namespace details
{

void os_sleep_for(const osal::chrono::ticks &sleep_duration);

template <class Clock>
void os_sleep_until(const std::chrono::time_point<Clock> &wakeup_time);

} // namespace details

} // namespace this_task

} // namespace osal

#endif
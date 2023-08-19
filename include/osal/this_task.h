#ifndef _OSAL_THIS_TASK_H_
#define _OSAL_THIS_TASK_H_

#include <chrono>

namespace osal
{

namespace this_task
{

void yield() noexcept;

template <class Rep, class Period>
void sleep_for(const std::chrono::duration<Rep, Period> &sleep_duration);

template <class Clock, class Duration>
bool sleep_until(const std::chrono::time_point<Clock, Duration> &wakeup_time);

}

}

#endif
#ifndef _OSAL_INCLUDE_OSAL_OS_CLOCK_H_
#define _OSAL_INCLUDE_OSAL_OS_CLOCK_H_

#include <cstdbool>
#include <cstdint>
#include <ratio>
#include <chrono>
#include "osal/osal_config.h"

namespace osal
{

namespace chrono
{

namespace details
{

template <typename Rep = configOSAL_TICK_TYPE,
          uint64_t TickRate = configOSAL_TICK_RATE_HZ>
class os_clock
{
public:
    using rep = Rep;
    using period = std::ratio<1, TickRate>;
    using duration = std::chrono::duration<os_clock::rep, os_clock::period>;
    using time_point = std::chrono::time_point<os_clock>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept;

    static duration current_tick() noexcept
    {
        return to_ticks(now());
    }

    static duration to_ticks(const time_point &time) noexcept
    {
        return time.time_since_epoch();
    }
};

} // namespace details

using steady_clock = details::os_clock<>;
using ticks = steady_clock::duration;

} // namespace chrono

} // namespace osal

#endif

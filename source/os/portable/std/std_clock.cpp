#include "osal/os_clock.h"
#include <chrono>

namespace osal
{

namespace chrono
{

using std_clock = details::os_clock<std::chrono::steady_clock::rep, std::chrono::steady_clock::period::den>;

std_clock::time_point std_clock::now() noexcept
{
    return std_clock::time_point(
            std::chrono::steady_clock::now().time_since_epoch());
}

}

}

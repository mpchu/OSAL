#include "osal/os_clock.h"
#include <chrono>

namespace osal
{

namespace chrono
{

using std_clock = details::os_clock<std::chrono::system_clock::rep, std::chrono::system_clock::period::den>;

std_clock::time_point std_clock::now() noexcept
{
    return std_clock::time_point(
            std::chrono::system_clock::now().time_since_epoch());
}

}

}

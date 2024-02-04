#include "osal/os_mutex.h"
#include <mutex>

namespace osal
{

using std_mutex = details::os_mutex<std::timed_mutex *>;

template<>
std_mutex::os_mutex() : _handle(new std::timed_mutex)
{
}

template<>
std_mutex::~os_mutex()
{
    delete _handle;
}

template<>
void std_mutex::lock()
{
    _handle->lock();
}

template<>
bool std_mutex::try_lock()
{
    return _handle->try_lock();
}

template<>
void std_mutex::unlock()
{
    _handle->unlock();
}

template<>
bool std_mutex::impl_try_lock_for(const osal::chrono::ticks &timeout)
{
    return _handle->try_lock_for(timeout);
}

}

// Instantiate the template specialization for this OS
template osal::std_mutex;
#include "osal/os_binary_semaphore.h"
#include "os/portable/std/internal/std_semaphore_handle.h"

namespace osal
{

using std_binary_semaphore = details::os_binary_semaphore<details::std_semaphore_handle *>;

template<>
std_binary_semaphore::os_binary_semaphore(bool desired)
    : _handle(nullptr)
{
    _handle = new details::std_semaphore_handle(1, desired);
}

template<>
std_binary_semaphore::~os_binary_semaphore()
{
    delete _handle;
}

template<>
void std_binary_semaphore::release()
{
    _handle->impl_release();
}

template<>
void std_binary_semaphore::acquire()
{
    _handle->impl_acquire();
}

template<>
bool std_binary_semaphore::try_acquire()
{
    return _handle->impl_try_acquire();
}

template<>
bool std_binary_semaphore::impl_try_acquire_for(const osal::chrono::ticks &timeout)
{
    return _handle->impl_try_acquire_for(timeout);
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::std_binary_semaphore;

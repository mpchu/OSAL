#include "osal/os_counting_semaphore.h"
#include "os/portable/std/internal/std_semaphore_handle.h"

namespace osal
{

using std_counting_semaphore = details::os_counting_semaphore<details::std_semaphore_handle *>;

template<>
std_counting_semaphore::os_counting_semaphore(std::size_t max_count, std::size_t initial) : _max_count(max_count)
{
    _handle = new details::std_semaphore_handle(_max_count, initial);
}

template<>
std_counting_semaphore::~os_counting_semaphore()
{
    delete _handle;
}

template<>
void std_counting_semaphore::release(std::size_t update)
{
    _handle->impl_release(update);
}

template<>
void std_counting_semaphore::acquire()
{
    _handle->impl_acquire();
}

template<>
bool std_counting_semaphore::try_acquire()
{
    return _handle->impl_try_acquire();
}

template<>
bool std_counting_semaphore::impl_try_acquire_for(const std::chrono::nanoseconds &timeout)
{
    return _handle->impl_try_acquire_for(timeout);
}

} // namespace osal

// Instantiate the template specialization for this OS
template osal::std_counting_semaphore;

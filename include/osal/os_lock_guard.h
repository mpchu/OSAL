#ifndef _OSAL_OS_LOCK_GUARD_H_
#define _OSAL_OS_LOCK_GUARD_H_

namespace osal
{

/**
 * @brief Portable mutex wrapper object that provides a scoped based mutex acquisition mechanism.
 * @tparam Lockable The mutex type that osal::lock_guard will lock.
 */
template <class Lockable>
class lock_guard
{
    Lockable &_mutex;

public:
    using mutex_type = Lockable; /**< Mutex type */

    /**
     * @brief lock_guard constructor. Acquires ownership of the given mutex.
     * @param[in] m Mutex to lock
     */
    explicit lock_guard(mutex_type &m) : _mutex(m) { _mutex.lock(); }

    /**
     * @brief lock_guard destructor. Releases ownership of the owned mutex on destruction.
     */
    ~lock_guard() { _mutex.unlock(); }

    template <class Lockable>
    lock_guard(const osal::lock_guard<Lockable> &rhs) = delete;
};

} // namespace osal

#endif
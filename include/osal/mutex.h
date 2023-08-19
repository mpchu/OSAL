#ifndef _OSAL_MUTEX_H_
#define _OSAL_MUTEX_H_

namespace osal
{

template <typename handle_t>
class mutex
{
    handle_t _mut_handle;

public:
    using native_handle_type = handle_t;

    mutex() = default;

    ~mutex() = default;

    mutex(const osal::mutex &rhs) = default;

    mutex(osal::mutex &&rhs) = default;

    osal::mutex &operator=(const osal::mutex &rhs) = default;

    void lock();

    bool try_lock();

    void unlock();

    mutex::native_handle_type native_handle() { return _mut_handle; }
};

}

#endif
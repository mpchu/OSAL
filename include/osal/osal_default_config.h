#ifndef _OSAL_INCLUDE_OSAL_DEFAULT_CONFIG_H_
#define _OSAL_INCLUDE_OSAL_DEFAULT_CONFIG_H_

#include <cstdint>

#ifndef configOSAL_MAXIMUM_TASK_NAME_SIZE
    #define configOSAL_MAXIMUM_TASK_NAME_SIZE 64
#endif

#ifndef configOSAL_MAXIMUM_QUEUE_NAME_SIZE
    #define configOSAL_MAXIMUM_QUEUE_NAME_SIZE 64
#endif

#ifndef configOSAL_MAXIMUM_TASK_PRIORITY
    #define configOSAL_MAXIMUM_TASK_PRIORITY INT32_MAX
#endif

#if defined(_WIN32) || defined(__linux__)
    #define configOSAL_HAS_STD_CONCURRENCY_SUPPORT_LIB 1
#else
    #define configOSAL_HAS_STD_CONCURRENCY_SUPPORT_LIB 0
#endif

#ifndef configOSAL_SEMAPHORE_NATIVE_HANDLE
    #if configOSAL_HAS_STD_CONCURRENCY_SUPPORT_LIB
        namespace osal{namespace details{class std_semaphore_handle;}}
        #define configOSAL_SEMAPHORE_NATIVE_HANDLE osal::details::std_semaphore_handle*
    #else
        #error "configOSAL_SEMAPHORE_NATIVE_HANDLE is not defined for this platform"
    #endif
#endif

#ifndef configOSAL_MUTEX_NATIVE_HANDLE
    #if configOSAL_HAS_STD_CONCURRENCY_SUPPORT_LIB
        #include <mutex>
        #define configOSAL_MUTEX_NATIVE_HANDLE std::timed_mutex*
    #else
        #error "configOSAL_MUTEX_NATIVE_HANDLE is not defined for this platform"
    #endif
#endif

#ifndef configOSAL_MSG_QUEUE_NATIVE_HANDLE
    #if configOSAL_HAS_STD_CONCURRENCY_SUPPORT_LIB
        namespace osal{namespace details{class std_queue_handle;}}
        #define configOSAL_MSG_QUEUE_NATIVE_HANDLE osal::details::std_queue_handle*
    #else
        #error "configOSAL_MSG_QUEUE_NATIVE_HANDLE is not defined for this platform"
    #endif
#endif

#ifndef configOSAL_TASK_NATIVE_HANDLE
    #if configOSAL_HAS_STD_CONCURRENCY_SUPPORT_LIB
        #include <thread>
        #define configOSAL_TASK_NATIVE_HANDLE std::thread*
    #else
        #error "configOSAL_TASK_NATIVE_HANDLE is not defined for this platform"
    #endif
#endif

#endif
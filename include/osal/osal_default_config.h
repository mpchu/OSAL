#ifndef _OSAL_DEFAULT_CONFIG_H_
#define _OSAL_DEFAULT_CONFIG_H_

namespace osal
{

using string_view = const char *;

#ifndef configOSAL_MAXIMUM_TASK_NAME_SIZE
    #define configOSAL_MAXIMUM_TASK_NAME_SIZE 64
#endif

#ifndef configOSAL_MAXIMUM_QUEUE_NAME_SIZE
    #define configOSAL_MAXIMUM_QUEUE_NAME_SIZE 64
#endif

#ifndef configOSAL_MAXIMUM_TASK_PRIORITY
    #define configOSAL_MAXIMUM_TASK_PRIORITY 0x7FFFFFFF
#endif

#if defined(_WIN32) || defined(__linux__)
    #define configOSAL_HAS_STD_THREAD_SUPPORT 1
#else
    #define configOSAL_HAS_STD_THREAD_SUPPORT 0
#endif

#ifndef configOSAL_SEMAPHORE_NATIVE_HANDLE
    #if configOSAL_HAS_STD_THREAD_SUPPORT
        #if __cplusplus >= 202002L
            #include <semaphore>
            #define configOSAL_SEMAPHORE_NATIVE_HANDLE std::counting_semaphore::native_handle_type
        #endif
    #else
        #error "configOSAL_SEMAPHORE_NATIVE_HANDLE is not defined for this platform!"
    #endif
#endif

}

#endif
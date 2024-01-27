#ifndef _OSAL_DEFAULT_CONFIG_H_
#define _OSAL_DEFAULT_CONFIG_H_

namespace osal
{

#if __cplusplus >= 201703L
    #include <string_view>
    using string_view = std::string_view;
#else
    using string_view = const char *;
#endif

#ifndef configOSAL_MAXIMUM_TASK_NAME_SIZE
    #define configOSAL_MAXIMUM_TASK_NAME_SIZE 64
#endif

#ifndef configOSAL_MAXIMUM_QUEUE_NAME_SIZE
    #define configOSAL_MAXIMUM_QUEUE_NAME_SIZE 64
#endif

#ifndef configOSAL_MAXIMUM_TASK_PRIORITY
    #define configOSAL_MAXIMUM_TASK_PRIORITY 0x7FFFFFFF
#endif

#if defined(WIN32) || defined(__linux__)
#endif

#ifndef configOSAL_SEMAPHORE_NATIVE_HANDLE
    #if __cplusplus >= 202002L
        #include <semaphore>
        #define configOSAL_SEMAPHORE_NATIVE_HANDLE std::counting_semaphore::native_handle_type
    #endif
#endif

}

#endif
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

}

#endif
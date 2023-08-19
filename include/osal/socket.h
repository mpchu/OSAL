#ifndef _OSAL_SOCKET_H_
#define _OSAL_SOCKET_H_

namespace osal
{

enum class DomainType : int
{
    INVALID = -1,
    INET    = 0,
    INET6   = 1,
    MAXIMUM_VALUE
};

enum class SocketType : int
{
    INVALID   = -1,
    STREAM    = 0,
    DGRAM     = 1,
    SEQPACKET = 2,
    RAW       = 3,
    MAXIMUM_VALUE
};

enum class ProtocolType : int
{
    INVALID = -1,
    TCP     = 0,
    UDP     = 1,
    MAXIMUM_VALUE
};

template <typename fd_t>
class socket
{
    fd_t _file_descriptor;
    
};

}

#endif
#ifndef SOCKETTYPE_H
#define SOCKETTYPE_H

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <sys/socket.h>
#endif

namespace Nickvision::Network
{
    /**
     * @brief Type of sockets.
     */
    enum class SocketType
    {
        Stream = SOCK_STREAM,
        Datagram = SOCK_DGRAM,
#ifdef __linux__
        SequencedPacket = SOCK_SEQPACKET
#endif
    };
}

#endif // SOCKETTYPE_H

#ifndef ADDRESSFAMILY_H
#define ADDRESSFAMILY_H

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <sys/socket.h>
#endif

namespace Nickvision::Network
{
    /**
     * @brief Type of address that can be used by a socket.
     */
    enum class AddressFamily
    {
#ifdef _WIN32
        Pipe,
#else
        Unix = AF_UNIX,
#endif
        IPv4 = AF_INET
    };
}

#endif //ADDRESSFAMILY_H

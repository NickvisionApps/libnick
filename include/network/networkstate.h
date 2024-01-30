#ifndef NETWORKSTATE_H
#define NETWORKSTATE_H

namespace Nickvision::Network
{
    /**
     * @brief States of a network connection. 
     */
    enum class NetworkState
    {
        Disconnected = 0,
        ConnectedLocal,
        ConnectedGlobal
    };
}

#endif //NETWORKSTATE_H
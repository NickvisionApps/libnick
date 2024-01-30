#include "network/networkstatechangedeventargs.h"

namespace Nickvision::Network
{
    NetworkStateChangedEventArgs::NetworkStateChangedEventArgs(NetworkState state)
        : m_state{ state }
    {

    }

    NetworkState NetworkStateChangedEventArgs::getState() const
    {
        return m_state;
    }
}
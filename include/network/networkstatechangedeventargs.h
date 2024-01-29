#ifndef NETWORKSTATECHANGEDEVENTARGS_H
#define NETWORKSTATECHANGEDEVENTARGS_H

#include "networkstate.h"
#include "events/eventargs.h"

namespace Nickvision::Network
{
    /**
     * @brief Event args for when the network state is changed
     */
    class NetworkStateChangedEventArgs : public Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a NetworkStateChangedEventArgs.
         * @param state NetworkStae
         */
        NetworkStateChangedEventArgs(NetworkState state);
        /**
         * @brief Gets the network state.
         * @return NetworkState
         */
        NetworkState getState() const;

    private:
        NetworkState m_state;
    };
}

#endif //NETWORKSTATECHANGEDEVENTARGS_H
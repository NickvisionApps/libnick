/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * An event argument for when the network state is changed.
 */

#ifndef NETWORKSTATECHANGEDEVENTARGS_H
#define NETWORKSTATECHANGEDEVENTARGS_H

#include "networkstate.h"
#include "events/eventargs.h"

namespace Nickvision::Network
{
    /**
     * @brief An event argument for when the network state is changed.
     */
    class NetworkStateChangedEventArgs : public Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a NetworkStateChangedEventArgs.
         * @param state NetworkStae
         */
        NetworkStateChangedEventArgs(NetworkState state) noexcept;
        /**
         * @brief Gets the network state.
         * @return NetworkState
         */
        NetworkState getState() const noexcept;

    private:
        NetworkState m_state;
    };
}

#endif //NETWORKSTATECHANGEDEVENTARGS_H
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
 * States of a network connection.
 */

#ifndef NETWORKSTATE_H
#define NETWORKSTATE_H

namespace Nickvision::Network
{
    /**
     * @brief States of a network connection. 
     */
    enum class NetworkState
    {
        Disconnected = 0, ///< The network is disconnected.
        ConnectedLocal, ///< The network is connected locally.
        ConnectedGlobal ///< The network is connected globally.
    };
}

#endif //NETWORKSTATE_H
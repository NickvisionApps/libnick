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
 * An inter process communicator (server/client).
 */

#ifndef INTERPROCESSCOMMUNICATOR_H
#define INTERPROCESSCOMMUNICATOR_H

#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "events/event.h"
#include "events/parameventargs.h"
#include "network/socket.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::App
{
    /**
     * @brief An inter process communicator (server/client). 
     */
    class InterProcessCommunicator
    {
    public:
        /**
         * @brief Constructs an InterProcessCommunicator.
         * @brief If this is the first IPC instance for all processes, this instance will become an IPC server. Else, this instance will become an IPC client.
         * @param id The id of the process (Must be the same amongst all IPCs that must talk to each other)
         * @throw std::runtime_error Thrown if the client or server IPC cannot be created
         */
        InterProcessCommunicator(const std::string& id);
        /**
         * @brief Destructs an InterProcessCommunicator. 
         */
        ~InterProcessCommunicator();
        /**
         * @brief Gets the event for when a command is received. 
         * @brief This event is only triggered on IPC server instances.
         * @return The command received event
         */
        Events::Event<Events::ParamEventArgs<std::vector<std::string>>>& commandReceived();
        /**
         * @brief Gets whether or not this instance is an IPC server.
         * @return True if this instance is an IPC server, else false.
         */
        bool isServer() const;
        /**
         * @brief Gets whether or not this instance is an IPC client.
         * @return True if this instance is an IPC client, else false.
         */
        bool isClient() const;
        /**
         * @brief Communicates to the IPC server instance. 
         * @brief If this instance is the running server, commandReceived will be triggered with the passed arguments.
         * @param args The command-line arguments
         * @param exitIfClient Whether or not to exit this process if it is an IPC client
         * @return True if command-line arguments were sent to a server instance
         */
        bool communicate(const std::vector<std::string>& args, bool exitIfClient = false);

    private:
        /**
         * @brief Runs the IPC server loop.
         */
        void runServer();
        bool m_serverRunning;
        Events::Event<Events::ParamEventArgs<std::vector<std::string>>> m_commandReceived;
        std::thread m_server;
#ifdef _WIN32
        std::wstring m_path;
        HANDLE m_serverPipe;
#else
        std::string m_id;
        std::unique_ptr<Network::Socket> m_serverSocket;
#endif
    };
}

#endif //INTERPROCESSCOMMUNICATOR_H

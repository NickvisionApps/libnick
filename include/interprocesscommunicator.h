#ifndef INTERPROCESSCOMMUNICATOR_H
#define INTERPROCESSCOMMUNICATOR_H

#include <string>
#include <thread>
#include <vector>
#include "events/event.h"
#include "events/parameventargs.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <sys/un.h>
#endif

namespace Nickvision::Aura
{
	/**
	 * @brief An inter process communicator (server/client). 
	 */
	class InterProcessCommunicator
	{
	public:
		/**
		 * @brief Constructs an InterProcessCommunicator.
		 * If this is the first IPC instance for all processes, this instance will become an IPC server. Else, this instance will become an IPC client.
		 * @throw std::logic_error Thrown if Aura::init() was not called before creating the IPC
		 * @throw std::runtime_error Thrown if the client or server IPC cannot be created
		 */
		InterProcessCommunicator();
		/**
		 * @brief Destructs an InterProcessCommunicator. 
		 */
		~InterProcessCommunicator();
		/**
		 * @brief Gets the event for when a command is received. 
		 * This event is only triggered on IPC server instances.
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
		 * If this instance is the running server, commandReceived will be triggered with the passed arguments.
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
		std::jthread m_server;
		std::string m_path;
#ifdef _WIN32
		HANDLE m_serverPipe;
#elif defined(__linux__)
		struct sockaddr_un m_sockaddr;
		int m_serverSocket;
#endif
	};
}

#endif //INTERPROCESSCOMMUNICATOR_H
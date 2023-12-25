#ifndef IPCSERVER_H
#define IPCSERVER_H

#include <string>
#include <thread>
#include <vector>
#include "events/event.h"
#include "events/parameventargs.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::Aura
{
	/**
	 * @brief A server for inter-process communication.
	 */
	class IPCServer
	{
	public:
		/**
		 * @brief Constructs an IPCServer. 
		 */
		IPCServer();
		/**
		 * @brief Destructs an IPCServer. 
		 */
		~IPCServer();
		/**
		 * @brief Gets the event for when a command is received.
		 * @return The command received event
		 */
		Events::Event<Events::ParamEventArgs<std::vector<std::string>>>& commandReceived();
		/**
		 * @brief Communicates to the IPCServer. 
		 * If a server is already running, the command-line arguments will be sent to the running server. Else, a new server will be started.
		 * @param args The command-line arguments
		 * @return True if command-line arguments were sent to a server, else false signifing a new server was started
		 */
		bool communicate(const std::vector<std::string>& args);

	private:
		std::string m_id;
		bool m_running;
		Events::Event<Events::ParamEventArgs<std::vector<std::string>>> m_commandReceived;
		std::jthread m_server;
#ifdef _WIN32
		std::string m_pipeName;
		OVERLAPPED m_overlapped;
#endif
		void runServer();
	};
}

#endif //IPCSERVER_H
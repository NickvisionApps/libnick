#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "networkstatechangedeventargs.h"
#include "events/event.h"

namespace Nickvision::Aura::Network
{
	/**
	 * @brief An object to monitor the state of the system's network connection. 
	 */
	class NetworkMonitor
	{
	public:
		/**
		 * @brief Constructs a NetworkMonitor. 
		 */
		NetworkMonitor();
		/**
		 * @brief Destructs a NetworkMonitor. 
		 */
		~NetworkMonitor();
		/**
		 * @brief Gets the StateChanged event. This event is invoked whenever the state of the network connection changes.
		 * @return The StateChanged event
		 */
		Events::Event<NetworkStateChangedEventArgs>& stateChanged();
		/**
		 * @brief Gets the state of the network connection.
		 * @return NetworkState
		 */
		NetworkState getConnectionState() const;
		/**
		 * @brief Manually checks for the state of the system's network connection. If a change is detected, the StateChanged even will be invoked.
		 */
		void checkConnectionState();

	private:
		Events::Event<NetworkStateChangedEventArgs> m_stateChanged;
		NetworkState m_connectionState;
#ifdef __linux__
		unsigned long m_networkChangedHandlerId;
#endif
	};
}

#endif //NETWORKMONITOR_H
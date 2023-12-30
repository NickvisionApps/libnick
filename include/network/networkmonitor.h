#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <mutex>
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
		 * @brief Constructs a NetworkMonitor. This method will call checkConnectionState() to get the initial system network state.
		 */
		NetworkMonitor() noexcept;
		/**
		 * @brief Destructs a NetworkMonitor. 
		 */
		~NetworkMonitor() noexcept;
		/**
		 * @brief Gets the StateChanged event. This event is invoked whenever the state of the network connection changes.
		 * @return The StateChanged event
		 */
		Events::Event<NetworkStateChangedEventArgs>& stateChanged() noexcept;
		/**
		 * @brief Gets the state of the network connection.
		 * @return NetworkState
		 */
		NetworkState getConnectionState() const noexcept;
		/**
		 * @brief Manually checks the state of the system's network connection. If a change is detected, the StateChanged event will be invoked.
		 */
		void checkConnectionState() noexcept;

	private:
		mutable std::mutex m_mutex;
		Events::Event<NetworkStateChangedEventArgs> m_stateChanged;
		NetworkState m_connectionState;
#ifdef __linux__
		unsigned long m_networkChangedHandlerId;
#endif
	};
}

#endif //NETWORKMONITOR_H
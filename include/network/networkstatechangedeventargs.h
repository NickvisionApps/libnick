#ifndef NETWORKSTATECHANGEDEVENTARGS_H
#define NETWORKSTATECHANGEDEVENTARGS_H

#include "networkstate.h"
#include "events/eventargs.h"

namespace Nickvision::Aura::Network
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
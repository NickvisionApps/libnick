#include "network/networkstatechangedeventargs.h"

namespace Nickvision::Aura::Network
{
	NetworkStateChangedEventArgs::NetworkStateChangedEventArgs(NetworkState state) noexcept
		: m_state{ state }
	{

	}

	NetworkState NetworkStateChangedEventArgs::getState() const noexcept
	{
		return m_state;
	}
}
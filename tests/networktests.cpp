#include <gtest/gtest.h>
#include "aura.h"
#include "network/networkmonitor.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Network;

TEST(NetworkTests, ConnectedGlobal)
{
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		ASSERT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}

TEST(NetworkTests, DisableNetCheck)
{
	ASSERT_TRUE(Aura::setEnvVar("AURA_DISABLE_NETCHECK", "true"));
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		ASSERT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}
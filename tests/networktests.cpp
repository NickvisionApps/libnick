#include <gtest/gtest.h>
#include <cstdlib>
#include "network/networkmonitor.h"

using namespace Nickvision::Aura::Network;

TEST(NetworkTests, ConnectedGlobal)
{
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		EXPECT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}

TEST(NetworkTests, DisableNetCheck)
{
	putenv("AURA_DISABLE_NETCHECK=true");
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		EXPECT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}
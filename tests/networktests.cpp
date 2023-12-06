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
	std::string disableNetCheck{ "AURA_DISABLE_NETCHECK=true" };
	EXPECT_EQ(putenv(disableNetCheck.c_str()), 0);
	NetworkMonitor netmon;
	netmon.stateChanged() += [](const NetworkStateChangedEventArgs& e)
	{
		EXPECT_EQ(e.getState(), NetworkState::ConnectedGlobal);
	};
	netmon.checkConnectionState();
}
#include <gtest/gtest.h>
#include "aura/aura.h"
#include "network/networkmonitor.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Network;

TEST(NetworkTests, ConnectedGlobal)
{
	NetworkMonitor netmon;
	ASSERT_EQ(netmon.getConnectionState(), NetworkState::ConnectedGlobal);
}

TEST(NetworkTests, DisableNetCheck)
{
	ASSERT_TRUE(Aura::getActive().setEnvVar("AURA_DISABLE_NETCHECK", "true"));
	NetworkMonitor netmon;
	ASSERT_EQ(netmon.getConnectionState(), NetworkState::ConnectedGlobal);
}
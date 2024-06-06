#include <gtest/gtest.h>
#include "network/networkmonitor.h"
#include "system/environment.h"

using namespace Nickvision::Network;
using namespace Nickvision::System;

TEST(NetworkTests, ConnectedGlobal)
{
    NetworkMonitor netmon;
    ASSERT_EQ(netmon.getConnectionState(), NetworkState::ConnectedGlobal);
}

TEST(NetworkTests, DisableNetCheck)
{
    ASSERT_TRUE(Environment::setVariable("AURA_DISABLE_NETCHECK", "true"));
    NetworkMonitor netmon;
    ASSERT_EQ(netmon.getConnectionState(), NetworkState::ConnectedGlobal);
}
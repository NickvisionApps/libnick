#include <gtest/gtest.h>
#include "network/networkmonitor.h"
#include "network/webclient.h"
#include "system/environment.h"

using namespace Nickvision::Network;
using namespace Nickvision::System;

class NetworkTest : public testing::Test
{
public:
    static std::unique_ptr<WebClient> m_webClient;

    static void SetUpTestSuite()
    {
        m_webClient = std::make_unique<WebClient>();
    }
};

std::unique_ptr<WebClient> NetworkTest::m_webClient = nullptr;

TEST_F(NetworkTest, ConnectedGlobal)
{
    NetworkMonitor netmon;
    ASSERT_EQ(netmon.getConnectionState(), NetworkState::ConnectedGlobal);
}

TEST_F(NetworkTest, DisableNetCheck)
{
    ASSERT_TRUE(Environment::setVariable("AURA_DISABLE_NETCHECK", "true"));
    NetworkMonitor netmon;
    ASSERT_EQ(netmon.getConnectionState(), NetworkState::ConnectedGlobal);
}

TEST_F(NetworkTest, ValidWebsite1)
{
    ASSERT_TRUE(m_webClient->getWebsiteExists("https://example.com"));
}

TEST_F(NetworkTest, ValidWebsite2)
{
    ASSERT_FALSE(m_webClient->getWebsiteExists("https://www.sdfjsdfj.com"));
}

TEST_F(NetworkTest, DownloadFile1)
{
    ASSERT_TRUE(m_webClient->downloadFile("https://raw.githubusercontent.com/nlogozzo/nlogozzo/main/README.md", "readme.md"));
    ASSERT_TRUE(std::filesystem::remove("readme.md"));
}

TEST_F(NetworkTest, FetchJsonString1)
{
    std::string s{ m_webClient->fetchJson("https://api.github.com/repos/nickvisionapps/denaro/tags") };
    ASSERT_TRUE(!s.empty());
}
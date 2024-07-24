#include <gtest/gtest.h>
#include "network/dns.h"
#include "network/ipv4address.h"
#include "network/macaddress.h"
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

TEST_F(NetworkTest, IPv4Address1)
{
    IPv4Address ip{ 192, 168, 1, 1 };
    ASSERT_EQ(ip.getFirst(), 192);
    ASSERT_EQ(ip.getSecond(), 168);
    ASSERT_EQ(ip.getThird(), 1);
    ASSERT_EQ(ip.getFourth(), 1);
    ASSERT_EQ(ip.str(), "192.168.1.1");
}

TEST_F(NetworkTest, IPv4Address2)
{
    std::optional<IPv4Address> ip{ IPv4Address::parse("76.98.1.193") };
    ASSERT_TRUE(ip.has_value());
    ASSERT_EQ(ip->getFirst(), 76);
    ASSERT_EQ(ip->getSecond(), 98);
    ASSERT_EQ(ip->getThird(), 1);
    ASSERT_EQ(ip->getFourth(), 193);
    ASSERT_EQ(ip->str(), "76.98.1.193");
}

TEST_F(NetworkTest, IPv4Address3)
{
    std::optional<IPv4Address> ip{ IPv4Address::parse("a.b.c.d") };
    ASSERT_FALSE(ip.has_value());
}

TEST_F(NetworkTest, IPv4Address4)
{
    IPv4Address ip{ 0x7F000001 };
    ASSERT_EQ(ip.getFirst(), 127);
    ASSERT_EQ(ip.getSecond(), 0);
    ASSERT_EQ(ip.getThird(), 0);
    ASSERT_EQ(ip.getFourth(), 1);
    ASSERT_EQ(ip.str(), "127.0.0.1");
}

TEST_F(NetworkTest, MacAddress1)
{
    MacAddress mac{ 0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E };
    ASSERT_EQ(mac.getFirst(), 0x00);
    ASSERT_EQ(mac.getSecond(), 0x1A);
    ASSERT_EQ(mac.getThird(), 0x2B);
    ASSERT_EQ(mac.getFourth(), 0x3C);
    ASSERT_EQ(mac.getFifth(), 0x4D);
    ASSERT_EQ(mac.getSixth(), 0x5E);
    ASSERT_EQ(mac.str(), "00:1A:2B:3C:4D:5E");
}

TEST_F(NetworkTest, MacAddress2)
{
    std::optional<MacAddress> mac{ MacAddress::parse("00:1A:2B:3C:4D:5E") };
    ASSERT_TRUE(mac.has_value());
    ASSERT_EQ(mac->getFirst(), 0x00);
    ASSERT_EQ(mac->getSecond(), 0x1A);
    ASSERT_EQ(mac->getThird(), 0x2B);
    ASSERT_EQ(mac->getFourth(), 0x3C);
    ASSERT_EQ(mac->getFifth(), 0x4D);
    ASSERT_EQ(mac->getSixth(), 0x5E);
    ASSERT_EQ(mac->str(), "00:1A:2B:3C:4D:5E");
}

TEST_F(NetworkTest, MacAddress3)
{
    std::optional<MacAddress> mac{ MacAddress::parse("00:1A:2B:3C:4D") };
    ASSERT_FALSE(mac.has_value());
}

TEST_F(NetworkTest, Dns1)
{
    ASSERT_FALSE(DNS::resolve("www.google.com").empty());
}
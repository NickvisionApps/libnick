#include <gtest/gtest.h>
#include "network/dns.h"

using namespace Nickvision::Network;

TEST(DnsTests, A1)
{
    ASSERT_FALSE(DNS::a("www.google.com").empty());
}

TEST(DnsTests, TXT1)
{
    ASSERT_FALSE(DNS::txt("example.com").empty());
}
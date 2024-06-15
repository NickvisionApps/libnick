#include <gtest/gtest.h>
#include "helpers/codehelpers.h"
#include "helpers/stringhelpers.h"
#include "network/webclient.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::Network;

TEST(StringTests, Upper1)
{
    ASSERT_EQ(StringHelpers::upper("abc"), "ABC");
}

TEST(StringTests, Upper2)
{
    ASSERT_EQ(StringHelpers::upper("uy7tG8"), "UY7TG8");
}

TEST(StringTests, Lower1)
{
    ASSERT_EQ(StringHelpers::lower("DEF"), "def");
}

TEST(StringTests, Lower2)
{
    ASSERT_EQ(StringHelpers::lower("uy7tG8"), "uy7tg8");
}

TEST(StringTests, Trim1)
{
    ASSERT_EQ(StringHelpers::trim("     abc        "), "abc");
}

TEST(StringTests, Trim2)
{
    ASSERT_EQ(StringHelpers::trim("     abc f "), "abc f");
}

TEST(StringTests, Trim3)
{
    ASSERT_EQ(StringHelpers::trim("---abc-rf---", '-'), "abc-rf");
}

TEST(StringTests, Split1)
{
    std::vector<std::string> cmd{ StringHelpers::split("libnick -t fg -y", " ")};
    ASSERT_EQ(cmd.size(), 4);
    ASSERT_EQ(cmd[0], "libnick");
    ASSERT_EQ(cmd[1], "-t");
    ASSERT_EQ(cmd[2], "fg");
    ASSERT_EQ(cmd[3], "-y");
}

TEST(StringTests, Guid1)
{
    std::string s;
    ASSERT_NO_THROW(s = StringHelpers::newGuid());
    ASSERT_FALSE(s.empty());
    ASSERT_TRUE(s.size() == 36);
}

TEST(StringTests, UrlValidity1)
{
    ASSERT_TRUE(StringHelpers::isValidUrl("https://www.google.com"));
}

TEST(StringTests, UrlValidity2)
{
    ASSERT_TRUE(StringHelpers::isValidUrl("http://youtube.com/signin"));
}

TEST(StringTests, UrlValidity3)
{
    ASSERT_TRUE(StringHelpers::isValidUrl("https://microsoft"));
}

TEST(StringTests, UrlValidity4)
{
    ASSERT_FALSE(StringHelpers::isValidUrl("asderm"));
}

TEST(StringTests, Join1)
{
    ASSERT_EQ(StringHelpers::join({ "hi", "bye" }, "|"), "hi|bye|");
}

TEST(StringTests, Join2)
{
    ASSERT_EQ(StringHelpers::join({ "hi", "bye" }, "|", false), "hi|bye");
}

TEST(StringTests, Join3)
{
    ASSERT_EQ(StringHelpers::join({ "hi" }, "\n"), "hi\n");
}

TEST(StringTests, Replace1)
{
    ASSERT_EQ(StringHelpers::replace("hello bye hi", "bye", "goodbye"), "hello goodbye hi");
}

TEST(StringTests, Replace2)
{
    ASSERT_EQ(StringHelpers::replace("hello bye hi bye", "bye", "goodbye"), "hello goodbye hi goodbye");
}

TEST(StringTests, Base641)
{
    std::vector<std::byte> s{ std::byte{'A'}, std::byte{'B'}, std::byte{'X'}, std::byte{'J'}, std::byte{'K'} };
    std::string base64;
    ASSERT_NO_THROW(base64 = StringHelpers::encode(s));
    ASSERT_FALSE(base64.empty());
    std::vector<std::byte> bytes;
    ASSERT_NO_THROW(bytes = StringHelpers::decode(base64));
    ASSERT_EQ(bytes.size(), s.size());
    for(size_t i = 0; i < bytes.size(); i++)
    {
        ASSERT_EQ(bytes[i], s[i]);
    }
}

TEST(StringTests, Base642)
{
    std::vector<std::byte> s{ std::byte{5}, std::byte{89}, std::byte{34}, std::byte{112}, std::byte{9}, std::byte{34}, std::byte{12}, std::byte{7}, std::byte{32}, std::byte{67}, std::byte{98}, std::byte{45}, std::byte{67}, std::byte{234} };
    std::string base64;
    ASSERT_NO_THROW(base64 = StringHelpers::encode(s));
    ASSERT_FALSE(base64.empty());
    std::vector<std::byte> bytes;
    ASSERT_NO_THROW(bytes = StringHelpers::decode(base64));
    ASSERT_EQ(bytes.size(), s.size());
    for(size_t i = 0; i < bytes.size(); i++)
    {
        ASSERT_EQ(bytes[i], s[i]);
    }
}

TEST(StringTests, Base643)
{
    WebClient client;
    client.downloadFile("https://www.freeiconspng.com/thumbs/pin-png/pin-png-24.png", "img.png");
    std::vector<std::byte> s{ CodeHelpers::readFileBytes("img.png") };
    std::string base64;
    ASSERT_NO_THROW(base64 = StringHelpers::encode(s));
    ASSERT_FALSE(base64.empty());
    std::vector<std::byte> bytes;
    ASSERT_NO_THROW(bytes = StringHelpers::decode(base64));
    ASSERT_EQ(bytes.size(), s.size());
    for(size_t i = 0; i < bytes.size(); i++)
    {
        ASSERT_EQ(bytes[i], s[i]);
    }
    std::filesystem::remove("img.png");
}

TEST(StringTests, SToW1)
{
    ASSERT_EQ(StringHelpers::wstr("hello"), L"hello");
}

TEST(StringTests, SToW2)
{
    ASSERT_EQ(StringHelpers::wstr("goodbye my friend"), L"goodbye my friend");
}

TEST(StringTests, WToS1)
{
    ASSERT_EQ(StringHelpers::str(L"hello"), "hello");
}

TEST(StringTests, WToS2)
{
    ASSERT_EQ(StringHelpers::str(L"goodbye my friend"), "goodbye my friend");
}
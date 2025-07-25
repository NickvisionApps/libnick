#include <gtest/gtest.h>
#include "helpers/codehelpers.h"
#include "helpers/stringhelpers.h"
#include "network/web.h"

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

TEST(StringTests, Split2)
{
    std::vector<std::string> cmd{ StringHelpers::splitArgs("libnick -t 'fg sg' \"Hello World\"")};
    ASSERT_EQ(cmd.size(), 4);
    ASSERT_EQ(cmd[0], "libnick");
    ASSERT_EQ(cmd[1], "-t");
    ASSERT_EQ(cmd[2], "fg sg");
    ASSERT_EQ(cmd[3], "Hello World");
}

TEST(StringTests, Split3)
{
    std::vector<std::string> cmd{ StringHelpers::splitArgs("osascript -e 'display notification \"Message\" with title \"Title\"'")};
    ASSERT_EQ(cmd.size(), 3);
    ASSERT_EQ(cmd[0], "osascript");
    ASSERT_EQ(cmd[1], "-e");
    ASSERT_EQ(cmd[2], "display notification \"Message\" with title \"Title\"");
}

TEST(StringTests, Split4)
{
    std::vector<std::string> cmd{ StringHelpers::split("libnick -t fg -y", ' ')};
    ASSERT_EQ(cmd.size(), 4);
    ASSERT_EQ(cmd[0], "libnick");
    ASSERT_EQ(cmd[1], "-t");
    ASSERT_EQ(cmd[2], "fg");
    ASSERT_EQ(cmd[3], "-y");
}

TEST(StringTests, Uuid1)
{
    std::string s;
    ASSERT_NO_THROW(s = StringHelpers::newUuid());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), 36);
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
    ASSERT_FALSE(StringHelpers::isValidUrl("https://microsoft"));
}

TEST(StringTests, UrlValidity4)
{
    ASSERT_FALSE(StringHelpers::isValidUrl("asderm"));
}

TEST(StringTests, UrlValidity5)
{
    ASSERT_TRUE(StringHelpers::isValidUrl("https://www.youtube.com/watch?v=eCq3G6LGhe8"));
}

TEST(StringTests, UrlValidity6)
{
    ASSERT_TRUE(StringHelpers::isValidUrl("https://www.youtube.com/watch?v=UKwSQSFN4Nw&list=PLXJg25X-OulsVsnvZ7RVtSDW-id9_RzAO"));
}

TEST(StringTests, Join1)
{
    ASSERT_EQ(StringHelpers::join({ "hi", "bye" }, "|"), "hi|bye");
}

TEST(StringTests, Join2)
{
    ASSERT_EQ(StringHelpers::join({ "hi", "bye" }, "|", true), "hi|bye|");
}

TEST(StringTests, Join3)
{
    ASSERT_EQ(StringHelpers::join({ "hi" }, "\n"), "hi");
}

TEST(StringTests, Replace1)
{
    ASSERT_EQ(StringHelpers::replace("hello bye hi", "bye", "goodbye"), "hello goodbye hi");
}

TEST(StringTests, Replace2)
{
    ASSERT_EQ(StringHelpers::replace("hello bye hi bye", "bye", "goodbye"), "hello goodbye hi goodbye");
}

TEST(StringTests, Replace3)
{
    ASSERT_EQ(StringHelpers::replace("hello bye hi bye", 'b', 'g'), "hello gye hi gye");
}

TEST(StringTests, Normalize1)
{
#ifdef _WIN32
    ASSERT_EQ(StringHelpers::normalizeForFilename("*Help*.txt"), "_Help_.txt");
#else
    ASSERT_EQ(StringHelpers::normalizeForFilename("*Help*.txt"), "*Help*.txt");
    ASSERT_EQ(StringHelpers::normalizeForFilename("*Help*.txt", true), "_Help_.txt");
#endif
}

TEST(StringTests, Normalize2)
{
    ASSERT_EQ(StringHelpers::normalizeForFilename("Hello/my/friend.txt"), "Hello_my_friend.txt");
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
    ASSERT_TRUE(Web::downloadFile("https://upload.wikimedia.org/wikipedia/commons/thumb/4/47/PNG_transparency_demonstration_1.png/640px-PNG_transparency_demonstration_1.png", "img.png"));
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

#include <gtest/gtest.h>
#include "helpers/stringhelpers.h"

using namespace Nickvision;

TEST(StringTests, Upper1)
{
	ASSERT_EQ(StringHelpers::toUpper("abc"), "ABC");
}

TEST(StringTests, Upper2)
{
	ASSERT_EQ(StringHelpers::toUpper("uy7tG8"), "UY7TG8");
}

TEST(StringTests, Lower1)
{
	ASSERT_EQ(StringHelpers::toLower("DEF"), "def");
}

TEST(StringTests, Lower2)
{
	ASSERT_EQ(StringHelpers::toLower("uy7tG8"), "uy7tg8");
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
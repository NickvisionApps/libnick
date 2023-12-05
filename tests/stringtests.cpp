#include <gtest/gtest.h>
#include <iostream>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Aura;

TEST(StringTests, Upper1)
{
	EXPECT_EQ(StringHelpers::toUpper("abc"), "ABC");
}

TEST(StringTests, Upper2)
{
	EXPECT_EQ(StringHelpers::toUpper("uy7tG8"), "UY7TG8");
}

TEST(StringTests, Lower1)
{
	EXPECT_EQ(StringHelpers::toLower("DEF"), "def");
}

TEST(StringTests, Lower2)
{
	EXPECT_EQ(StringHelpers::toLower("uy7tG8"), "uy7tg8");
}

TEST(StringTests, Trim1)
{
	EXPECT_EQ(StringHelpers::trim("     abc        "), "abc");
}

TEST(StringTests, Trim2)
{
	EXPECT_EQ(StringHelpers::trim("     abc f "), "abc f");
}

TEST(StringTests, Trim3)
{
	EXPECT_EQ(StringHelpers::trim("---abc-rf---", '-'), "abc-rf");
}

TEST(StringTests, Split1)
{
	std::vector<std::string> cmd{ StringHelpers::split("libaura -t fg -y", " ")};
	EXPECT_EQ(cmd.size(), 4);
	EXPECT_EQ(cmd[0], "libaura");
	EXPECT_EQ(cmd[1], "-t");
	EXPECT_EQ(cmd[2], "fg");
	EXPECT_EQ(cmd[3], "-y");
}

TEST(StringTests, Guid1)
{
	EXPECT_NO_THROW(std::cout << StringHelpers::newGuid() << std::endl);
}

TEST(StringTests, UrlValidity1)
{
	EXPECT_TRUE(StringHelpers::isValidUrl("https://www.google.com"));
}

TEST(StringTests, UrlValidity2)
{
	EXPECT_TRUE(StringHelpers::isValidUrl("http://youtube.com/signin"));
}

TEST(StringTests, UrlValidity3)
{
	EXPECT_FALSE(StringHelpers::isValidUrl("https://microsoft"));
}

TEST(StringTests, UrlValidity4)
{
	EXPECT_FALSE(StringHelpers::isValidUrl("asderm"));
}
#include <gtest/gtest.h>
#include "version.h"

using namespace Nickvision::Aura;

TEST(VersionTests, EmptyVersion) 
{
	Version v;
	EXPECT_EQ(v.empty(), true);
}

TEST(VersionTests, VersionFromStrings)
{
	Version v1{ "2023.11.52" };
	Version v2{ "2022.15.2" };
	EXPECT_EQ(v1.toString(), "2023.11.52");
	EXPECT_EQ(v2.toString(), "2022.15.2");
	EXPECT_TRUE(v1 > v2);
	EXPECT_TRUE(v2 < v1);
	EXPECT_FALSE(v1 == v2);
}

TEST(VersionTests, BetaVersion1)
{
	Version v{ "2010.5.0-beta1" };
	EXPECT_EQ(v.getVersionType(), VersionType::Preview);
}

TEST(VersionTests, BadVersion1)
{
	EXPECT_THROW(Version(""), std::invalid_argument);
}

TEST(VersionTests, BadVersion2)
{
	EXPECT_THROW(Version("2023"), std::invalid_argument);
}

TEST(VersionTests, BadVersion3)
{
	EXPECT_THROW(Version("2023-11-0"), std::invalid_argument);
}
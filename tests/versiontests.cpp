#include <gtest/gtest.h>
#include "version.h"

using namespace Nickvision::Aura;

TEST(VersionTests, EmptyVersion) 
{
	Version v;
	ASSERT_EQ(v.empty(), true);
}

TEST(VersionTests, VersionFromStrings)
{
	Version v1{ "2023.11.52" };
	Version v2{ "2022.15.2" };
	ASSERT_EQ(v1.toString(), "2023.11.52");
	ASSERT_EQ(v2.toString(), "2022.15.2");
	ASSERT_TRUE(v1 > v2);
	ASSERT_TRUE(v2 < v1);
	ASSERT_FALSE(v1 == v2);
}

TEST(VersionTests, BetaVersion1)
{
	Version v{ "2010.5.0-beta1" };
	ASSERT_EQ(v.getVersionType(), VersionType::Preview);
}

TEST(VersionTests, BadVersion1)
{
	ASSERT_THROW(Version(""), std::invalid_argument);
}

TEST(VersionTests, BadVersion2)
{
	ASSERT_THROW(Version("2023"), std::invalid_argument);
}

TEST(VersionTests, BadVersion3)
{
	ASSERT_THROW(Version("2023-11-0"), std::invalid_argument);
}
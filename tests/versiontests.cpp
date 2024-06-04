#include <gtest/gtest.h>
#include "update/version.h"

using namespace Nickvision::Update;

TEST(VersionTests, EmptyVersion) 
{
    Version v;
    ASSERT_EQ(v.empty(), true);
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

TEST(VersionTests, CompareVersion1)
{
    Version v1{ "2023.2.0" };
    Version v2{ "2024.1.0" };
    ASSERT_TRUE(v1 < v2);
    ASSERT_TRUE(v2 > v1);
    ASSERT_FALSE(v1 == v2);
}

TEST(VersionTests, CompareVersion2)
{
    Version v1{ "2024.2.0" };
    Version v2{ "2024.2.0-beta1" };
    ASSERT_TRUE(v1 > v2);
    ASSERT_TRUE(v2 < v1);
    ASSERT_FALSE(v1 == v2);
}

TEST(VersionTests, CompareVersion3)
{
    Version v1{ "2024.6.0" };
    Version v2{ "2023.5.1" };
    ASSERT_TRUE(v1 > v2);
    ASSERT_TRUE(v2 < v1);
    ASSERT_FALSE(v1 == v2);
}

TEST(VersionTests, CompareVersion4)
{
    Version v1{ "2024.6.0" };
    Version v2{ "2024.6.0" };
    ASSERT_FALSE(v1 > v2);
    ASSERT_FALSE(v2 < v1);
    ASSERT_TRUE(v1 == v2);
}
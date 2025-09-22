#include <gtest/gtest.h>
#include "update/version.h"

using namespace Nickvision::Update;

TEST(VersionTests, EmptyVersion) 
{
    Version v;
    ASSERT_EQ(v.empty(), true);
}

TEST(VersionTests, StableVersion1)
{
    Version v{ "2025.9.0" };
    ASSERT_EQ(v.getVersionType(), VersionType::Stable);
    ASSERT_EQ(v.str(), "2025.9.0");
}

TEST(VersionTests, StableVersion2)
{
    Version v{ "0.2.0" };
    ASSERT_EQ(v.getVersionType(), VersionType::Stable);
    ASSERT_EQ(v.str(), "0.2.0");
}

TEST(VersionTests, StableVersion3)
{
    Version v{ "1.2.4" };
    ASSERT_EQ(v.getVersionType(), VersionType::Stable);
    ASSERT_EQ(v.str(), "1.2.4");
}

TEST(VersionTests, BetaVersion1)
{
    Version v{ "2010.5.0-beta1" };
    ASSERT_EQ(v.getVersionType(), VersionType::Preview);
    ASSERT_EQ(v.str(), "2010.5.0-beta1");
}

TEST(VersionTests, BetaVersion2)
{
    Version v{ "2025.9.1-next" };
    ASSERT_EQ(v.getVersionType(), VersionType::Preview);
    ASSERT_EQ(v.str(), "2025.9.1-next");
}

TEST(VersionTests, BetaVersion3)
{
    Version v{ "2025.9.1.beta1" };
    ASSERT_EQ(v.getVersionType(), VersionType::Preview);
    ASSERT_EQ(v.str(), "2025.9.1-beta1");
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
    Version v1{ "2025.9.0-beta1" };
    Version v2{ "2025.8.0" };
    ASSERT_TRUE(v1 > v2);
    ASSERT_TRUE(v2 < v1);
    ASSERT_FALSE(v1 == v2);
}

TEST(VersionTests, CompareVersion5)
{
    Version v1{ "2025.9.0-beta2" };
    Version v2{ "2025.9.0-beta1" };
    ASSERT_TRUE(v1 > v2);
    ASSERT_TRUE(v2 < v1);
    ASSERT_FALSE(v1 == v2);
}

#include <gtest/gtest.h>
#include <filesystem>
#include "helpers/webhelpers.h"

using namespace Nickvision;

TEST(WebTests, ValidWebsite1)
{
    ASSERT_TRUE(WebHelpers::isValidWebsite("https://example.com"));
}

TEST(WebTests, ValidWebsite2)
{
    ASSERT_FALSE(WebHelpers::isValidWebsite("https://www.sdfjsdfj.com"));
}

TEST(WebTests, DownloadFile1)
{
    ASSERT_TRUE(WebHelpers::downloadFile("https://raw.githubusercontent.com/nlogozzo/nlogozzo/main/README.md", "readme.md"));
    ASSERT_TRUE(std::filesystem::remove("readme.md"));
}

TEST(WebTests, FetchJsonString1)
{
    std::string s{ WebHelpers::fetchJsonString("https://api.github.com/repos/nickvisionapps/denaro/tags") };
    ASSERT_TRUE(!s.empty());
}
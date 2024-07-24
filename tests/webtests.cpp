#include <gtest/gtest.h>
#include <filesystem>
#include "network/web.h"

using namespace Nickvision::Network;

TEST(WebTests, ValidWebsite1)
{
    ASSERT_TRUE(Web::getWebsiteExists("https://example.com"));
}

TEST(WebTests, ValidWebsite2)
{
    ASSERT_FALSE(Web::getWebsiteExists("https://www.sdfjsdfj.com"));
}

TEST(WebTests, DownloadFile1)
{
    ASSERT_TRUE(Web::downloadFile("https://raw.githubusercontent.com/nlogozzo/nlogozzo/main/README.md", "readme.md"));
    ASSERT_TRUE(std::filesystem::remove("readme.md"));
}

TEST(WebTests, FetchJsonString1)
{
    Json::Value json{ Web::fetchJson("https://api.github.com/repos/nickvisionapps/denaro/tags") };
    ASSERT_FALSE(json.empty());
}
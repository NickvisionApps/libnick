#include <gtest/gtest.h>
#include "helpers/webhelpers.h"

using namespace Nickvision::Aura;

TEST(WebTests, ValidWebsite1)
{
	EXPECT_TRUE(WebHelpers::isValidWebsite("https://example.com"));
}

TEST(WebTests, ValidWebsite2)
{
	EXPECT_FALSE(WebHelpers::isValidWebsite("https://www.sdfjsdfj.com"));
}

TEST(WebTests, DownloadFile1)
{
	EXPECT_TRUE(WebHelpers::downloadFile("https://raw.githubusercontent.com/nlogozzo/nlogozzo/main/README.md", "readme.md"));
}

TEST(WebTests, FetchJsonString1)
{
	std::string s{ WebHelpers::fetchJsonString("https://api.github.com/repos/nickvisionapps/denaro/tags") };
	std::cout << s << std::endl;
	EXPECT_TRUE(!s.empty());
}
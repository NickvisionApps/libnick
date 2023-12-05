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
	EXPECT_TRUE(WebHelpers::downloadFile("https://www.w3.org/TR/2003/REC-PNG-20031110/iso_8859-1.txt", "test.txt"));
}
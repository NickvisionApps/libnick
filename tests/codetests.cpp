#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <iterator>
#include "helpers/codehelpers.h"
#include "network/web.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::Network;

class CodeTest : public testing::Test
{
public:
    static std::vector<std::byte> s;

    static void SetUpTestSuite()
    {
        Web::downloadFile("https://www.freeiconspng.com/thumbs/pin-png/pin-png-24.png", "a.png");
    }

    static void TearDownTestSuite()
    {
        std::filesystem::remove("a.png");
        std::filesystem::remove("b.png");
    }
};

std::vector<std::byte> CodeTest::s = {};

TEST_F(CodeTest, GetFileABytes)
{
    s = CodeHelpers::readFileBytes("a.png");
    ASSERT_FALSE(s.empty());
}

TEST_F(CodeTest, WriteFileBBytes)
{
    ASSERT_TRUE(CodeHelpers::writeFileBytes("b.png", s));
}

TEST_F(CodeTest, CompareFileABytesToFileBBytes)
{
    std::ifstream fileA{ "a.png", std::ios::binary };
    std::ifstream fileB{ "b.png", std::ios::binary };
    ASSERT_TRUE(fileA.good());
    ASSERT_TRUE(fileB.good());
    fileA.seekg(0, std::ifstream::beg);
    fileB.seekg(0, std::ifstream::beg);
    ASSERT_TRUE(std::equal(std::istreambuf_iterator<char>(fileA.rdbuf()), std::istreambuf_iterator<char>(), std::istreambuf_iterator<char>(fileB.rdbuf())));
}

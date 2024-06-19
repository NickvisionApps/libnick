#include <gtest/gtest.h>
#include <memory>
#include "system/environment.h"
#include "system/suspendinhibitor.h"

using namespace Nickvision::System;

class SystemTest : public testing::Test
{
public:
    static std::unique_ptr<SuspendInhibitor> m_inhibitor;

    static void SetUpTestSuite()
    {
        m_inhibitor = std::make_unique<SuspendInhibitor>();
    }
};

std::unique_ptr<SuspendInhibitor> SystemTest::m_inhibitor = nullptr;

TEST_F(SystemTest, GetPath)
{
    ASSERT_FALSE(Environment::getVariable("PATH").empty());
}

TEST_F(SystemTest, SetTestVar)
{
    ASSERT_TRUE(Environment::setVariable("AURA_TEST_VAR", "test"));
}

TEST_F(SystemTest, EnsureTestVar)
{
    ASSERT_EQ(Environment::getVariable("AURA_TEST_VAR"), "test");
}

TEST_F(SystemTest, ClearTestVar)
{
    ASSERT_TRUE(Environment::clearVariable("AURA_TEST_VAR"));
}

TEST_F(SystemTest, Exec)
{
#ifdef _WIN32
    ASSERT_EQ(Environment::exec("echo Hello World"), "Hello World\r\n");
#else
    ASSERT_EQ(Environment::exec("echo Hello World"), "Hello World\n");
#endif
}

TEST_F(SystemTest, InhibitSuspend)
{
#ifdef __linux__
    if(!Environment::getVariable("GITHUB_ACTIONS").empty())
    {
        GTEST_SKIP();
    }
#endif
    ASSERT_TRUE(SystemTest::m_inhibitor->inhibit());
    ASSERT_TRUE(SystemTest::m_inhibitor->isInhibiting());
}

TEST_F(SystemTest, UninhibitSuspend)
{
#ifdef __linux__
    if(!Environment::getVariable("GITHUB_ACTIONS").empty())
    {
        GTEST_SKIP();
    }
#endif
    ASSERT_TRUE(SystemTest::m_inhibitor->uninhibit());
    ASSERT_FALSE(SystemTest::m_inhibitor->isInhibiting());
}
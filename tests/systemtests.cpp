#include <gtest/gtest.h>
#include "system/environment.h"

using namespace Nickvision::System;

TEST(SystemTests, GetPath)
{
    ASSERT_FALSE(Environment::getVariable("PATH").empty());
}

TEST(SystemTests, SetTestVar)
{
    ASSERT_TRUE(Environment::setVariable("AURA_TEST_VAR", "test"));
}

TEST(SystemTests, EnsureTestVar)
{
    ASSERT_EQ(Environment::getVariable("AURA_TEST_VAR"), "test");
}

TEST(SystemTests, ClearTestVar)
{
    ASSERT_TRUE(Environment::clearVariable("AURA_TEST_VAR"));
}

TEST(SystemTests, Exec)
{
#ifdef _WIN32
    ASSERT_EQ(Environment::exec("echo Hello World"), "Hello World\r\n");
#elif defined(__linux__)
    ASSERT_EQ(Environment::exec("echo Hello World"), "Hello World\n");
#endif
}
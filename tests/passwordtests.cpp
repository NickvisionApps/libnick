#include <gtest/gtest.h>
#include <iostream>
#include "keyring/passwordgenerator.h"

using namespace Nickvision::Keyring;

TEST(PasswordTests, Generator1)
{
    PasswordGenerator gen;
    std::string password{ gen.next() };
    std::cerr << "[INFO] Password generated: " << password << std::endl;
    ASSERT_TRUE(password.size() == 16);
}

TEST(PasswordTests, Generator2)
{
    PasswordGenerator gen{ PasswordContent::Numeric };
    std::string password{ gen.next(23) };
    std::cerr << "[INFO] Password generated: " << password << std::endl;
    ASSERT_TRUE(password.size() == 23);
}

TEST(PasswordTests, Generator3)
{
    PasswordGenerator gen{ PasswordContent::Numeric | PasswordContent::Lowercase };
    std::string password{ gen.next(12) };
    std::cerr << "[INFO] Password generated: " << password << std::endl;
    ASSERT_TRUE(password.size() == 12);
}

TEST(PasswordTests, Generator4)
{
    PasswordGenerator gen;
    std::string password{ gen.next(64) };
    std::cerr << "[INFO] Password generated: " << password << std::endl;
    ASSERT_TRUE(password.size() == 64);
}
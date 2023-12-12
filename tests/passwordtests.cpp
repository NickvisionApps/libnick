#include <gtest/gtest.h>
#include "keyring/passwordgenerator.h"
#include "keyring/passwordstrength.h"

using namespace Nickvision::Aura::Keyring;

TEST(PasswordTests, Strength1)
{
	ASSERT_TRUE(getPasswordStrength("") == PasswordStrength::Blank);
}

TEST(PasswordTests, Strength2)
{
	ASSERT_TRUE(getPasswordStrength("abc123") == PasswordStrength::VeryWeak);
}

TEST(PasswordTests, Strength3)
{
	ASSERT_TRUE(getPasswordStrength("abcdefg") == PasswordStrength::VeryWeak);
}

TEST(PasswordTests, Strength4)
{
	ASSERT_TRUE(getPasswordStrength("abc1234") == PasswordStrength::Weak);
}

TEST(PasswordTests, Strength5)
{
	ASSERT_TRUE(getPasswordStrength("rtyh6785sd32") == PasswordStrength::Medium);
}

TEST(PasswordTests, Strength6)
{
	ASSERT_TRUE(getPasswordStrength("asdh%^asdjhk123") == PasswordStrength::Strong);
}

TEST(PasswordTests, Strength7)
{
	ASSERT_TRUE(getPasswordStrength("aSDh%^asdjhk123") == PasswordStrength::VeryStrong);
}

TEST(PasswordTests, Generator1)
{
	PasswordGenerator gen;
	ASSERT_TRUE(gen.next().size() == 16);
}

TEST(PasswordTests, Generator2)
{
	PasswordGenerator gen{ PasswordContent::Numeric };
	ASSERT_TRUE(getPasswordStrength(gen.next()) == PasswordStrength::Weak);
}

TEST(PasswordTests, Generator3)
{
	PasswordGenerator gen{ PasswordContent::Numeric | PasswordContent::Lowercase };
	ASSERT_TRUE(getPasswordStrength(gen.next(4)) == PasswordStrength::VeryWeak);
}

TEST(PasswordTests, Generator4)
{
	PasswordGenerator gen;
	ASSERT_TRUE(getPasswordStrength(gen.next()) >= PasswordStrength::Medium);
}
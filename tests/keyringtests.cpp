#include <gtest/gtest.h>
#include <iostream>
#include "keyring/passwordgenerator.h"

using namespace Nickvision::Aura::Keyring;

TEST(KeyringTests, PasswordGen1)
{
	PasswordGenerator gen;
	std::string pass1 = gen.next();
	std::string pass2 = gen.next();
	std::string pass3 = gen.next(6);
	std::cout << pass1 << std::endl << pass2 << std::endl << pass3 << std::endl;
	EXPECT_EQ(pass1.length(), 16);
	EXPECT_EQ(pass2.length(), 16);
	EXPECT_EQ(pass3.length(), 6);
	EXPECT_FALSE(pass1 == pass2);
}
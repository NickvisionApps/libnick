#include <gtest/gtest.h>
#include <iostream>
#include "keyring/keyring.h"
#include "keyring/passwordgenerator.h"

using namespace Nickvision::Aura::Keyring;

class KeyringTest : public testing::Test 
{
public:
	PasswordGenerator m_passGen;
	std::shared_ptr<Keyring> m_keyring;

	KeyringTest()
	{
		std::optional<Keyring> keyring{ Keyring::access("org.nickvision.auratests") };
		if (keyring.has_value())
		{
			m_keyring = std::make_shared<Keyring>(keyring.value());
		}
	}
};

TEST_F(KeyringTest, KeyringInitalization)
{
	ASSERT_TRUE(m_keyring);
}

TEST_F(KeyringTest, AddCredential1)
{
	ASSERT_TRUE(m_keyring);
	ASSERT_TRUE(m_keyring->addCredential({ "YT", "https://youtube.com", "test1", m_passGen.next() }));
}

TEST_F(KeyringTest, AddCredential2)
{
	ASSERT_TRUE(m_keyring);
	ASSERT_TRUE(m_keyring->addCredential({ "GitHub", "https://github.com", "test2", m_passGen.next() }));
}

TEST_F(KeyringTest, FetchCredentials)
{
	ASSERT_TRUE(m_keyring);
	ASSERT_TRUE(m_keyring->getAllCredentials().size() == 2);
}

TEST_F(KeyringTest, KeyringDestroy)
{
	ASSERT_TRUE(m_keyring);
	ASSERT_TRUE(m_keyring->destroy());
}
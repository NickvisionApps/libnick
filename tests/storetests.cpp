#include <gtest/gtest.h>
#include <iostream>
#include "keyring/passwordgenerator.h"
#include "keyring/store.h"

using namespace Nickvision::Aura::Keyring;

class StoreTest : public testing::Test
{
public:
	static std::shared_ptr<Store> m_store;

	static void SetUpTestSuite()
	{
		Store::destroy("org.nickvision.aura.test");
		PasswordGenerator passGen;
		m_store = std::make_shared<Store>("org.nickvision.aura.test", passGen.next());
	}
};

std::shared_ptr<Store> StoreTest::m_store = nullptr;

TEST_F(StoreTest, CheckValidStore)
{
	ASSERT_TRUE(m_store->isValid());
}

TEST_F(StoreTest, AddCredentials)
{
	ASSERT_TRUE(m_store->isValid());
	ASSERT_TRUE(m_store->addCredential({ "YT", "https://youtube.com", "theawesomeguy", "abc123!" }));
	ASSERT_TRUE(m_store->addCredential({ "Google", "https://google.com", "me@gmail.com", "abc12345!" }));
}

TEST_F(StoreTest, EnsureCredentials)
{
	ASSERT_TRUE(m_store->isValid());
	ASSERT_TRUE(m_store->getAllCredentials().size() == 2);
	std::vector<Credential> creds{ m_store->getCredentials("YT") };
	ASSERT_TRUE(creds.size() == 1);
	std::cout << creds[0] << std::endl;
	ASSERT_EQ(creds[0].getName(), "YT");
	ASSERT_EQ(creds[0].getUri(), "https://youtube.com");
	ASSERT_EQ(creds[0].getUsername(), "theawesomeguy");
	ASSERT_EQ(creds[0].getPassword(), "abc123!");
}

TEST_F(StoreTest, DestroyStore)
{
	ASSERT_TRUE(m_store->isValid());
	ASSERT_TRUE(m_store->destroy());
}
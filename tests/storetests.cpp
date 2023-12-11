#include <gtest/gtest.h>
#include <iostream>
#include "keyring/passwordgenerator.h"
#include "keyring/store.h"

using namespace Nickvision::Aura::Keyring;

static const std::string storeName{ "AuraTestStore" };
static const std::string storePassword{ PasswordGenerator().next() };

TEST(StoreTest, EnsureNoStore)
{
	ASSERT_TRUE(Store::destroy(storeName));
}

TEST(StoreTests, CreateStrore)
{
	Store store{ storeName, storePassword };
	ASSERT_TRUE(store.isValid());
}

TEST(StoreTests, AddCredentials)
{
	Store store{ storeName, storePassword };
	ASSERT_TRUE(store.isValid());
	ASSERT_TRUE(store.addCredential({ "YT", "https://youtube.com", "theawesomeguy", "abc123!" }));
	ASSERT_TRUE(store.addCredential({ "Google", "https://google.com", "me@gmail.com", "abc12345!" }));
	ASSERT_TRUE(store.getAllCredentials().size() == 2);
	std::vector<Credential> creds{ store.getCredentials("YT") };
	ASSERT_TRUE(creds.size() == 1);
	std::cout << creds[0] << std::endl;
	ASSERT_EQ(creds[0].getName(), "YT");
	ASSERT_EQ(creds[0].getUri(), "https://youtube.com");
	ASSERT_EQ(creds[0].getUsername(), "theawesomeguy");
	ASSERT_EQ(creds[0].getPassword(), "abc123!");
}

TEST(StoreTests, DeleteStore)
{
	Store store{ storeName, storePassword };
	ASSERT_TRUE(store.isValid());
	ASSERT_TRUE(store.destroy());
}
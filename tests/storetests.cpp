#include <gtest/gtest.h>
#include "keyring/passwordgenerator.h"
#include "keyring/store.h"

using namespace Nickvision::Keyring;

class StoreTest : public testing::Test
{
public:
    static std::unique_ptr<Store> m_store;

    static void SetUpTestSuite()
    {
        Store::destroy("org.nickvision.aura.test");
    }
};

std::unique_ptr<Store> StoreTest::m_store = nullptr;

TEST_F(StoreTest, CreateStore)
{
    PasswordGenerator passGen;
    ASSERT_NO_THROW(m_store = std::make_unique<Store>("org.nickvision.aura.test", passGen.next()));
}

TEST_F(StoreTest, AddCredentials)
{
    ASSERT_TRUE(m_store->addCredential({ "YT", "https://youtube.com", "theawesomeguy", "abc123!" }));
    ASSERT_TRUE(m_store->addCredential({ "Google", "https://google.com", "me@gmail.com", "abc12345!" }));
    ASSERT_TRUE(m_store->getAllCredentials().size() == 2);
    std::vector<Credential> creds{ m_store->getCredentials("YT") };
    ASSERT_TRUE(creds.size() == 1);
    ASSERT_EQ(creds[0].getName(), "YT");
    ASSERT_EQ(creds[0].getUri(), "https://youtube.com");
    ASSERT_EQ(creds[0].getUsername(), "theawesomeguy");
    ASSERT_EQ(creds[0].getPassword(), "abc123!");
}

TEST_F(StoreTest, DestroyStore)
{
    ASSERT_TRUE(m_store->destroy());
}
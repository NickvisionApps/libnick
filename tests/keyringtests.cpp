#include <gtest/gtest.h>
#include <filesystem>
#include "filesystem/userdirectories.h"
#include "keyring/keyring.h"
#include "system/environment.h"

#define KEYRING_NAME std::string("org.nickvision.libnick.test")

using namespace Nickvision::Filesystem;
using namespace Nickvision::Keyring;
using namespace Nickvision::System;

class KeyringTest : public testing::Test
{
public:
    static std::unique_ptr<Keyring> m_keyring;
    static std::filesystem::path m_keyringPath;

    static void SetUpTestSuite()
    {
        std::filesystem::remove(m_keyringPath);
    }

    static void TearDownTestSuite()
    {
        m_keyring.reset();
        std::filesystem::remove(m_keyringPath);
    }
};

std::unique_ptr<Keyring> KeyringTest::m_keyring{ nullptr };
std::filesystem::path KeyringTest::m_keyringPath{ UserDirectories::get(UserDirectory::Config) / "Nickvision" / "Keyring" / (KEYRING_NAME + ".ring2") };

TEST_F(KeyringTest, Create)
{
    ASSERT_NO_THROW(m_keyring = std::make_unique<Keyring>(KEYRING_NAME));
#ifdef __APPLE__
    if(!Environment::hasVariable("GITHUB_ACTIONS"))
    {
        ASSERT_TRUE(m_keyring->isSavingToDisk());
    }
#else
    ASSERT_TRUE(m_keyring->isSavingToDisk());
#endif
}

TEST_F(KeyringTest, AddCredential1)
{
    Credential credential{ "YouTube", "https://youtube.com", "theawesomeguy", "abc123!" };
    ASSERT_TRUE(m_keyring->addCredential(credential));
    ASSERT_EQ(m_keyring->getCredentials().size(), 1);
    ASSERT_EQ(m_keyring->getCredential("YouTube"), credential);
}

TEST_F(KeyringTest, AddCredential2)
{
    Credential credential{ "GitHub", "https://github.com", "theawesomeguy", "abc123!" };
    ASSERT_TRUE(m_keyring->addCredential(credential));
    ASSERT_EQ(m_keyring->getCredentials().size(), 2);
    ASSERT_EQ(m_keyring->getCredential("GitHub"), credential);
}

TEST_F(KeyringTest, AddCredential3)
{
    Credential credential{ "YouTube", "https://youtube.com", "me@gmail.com", "abc123!" };
    ASSERT_FALSE(m_keyring->addCredential(credential));
}

TEST_F(KeyringTest, ReloadAndValidate)
{
    ASSERT_NO_THROW(m_keyring.reset());
    ASSERT_NO_THROW(m_keyring = std::make_unique<Keyring>(KEYRING_NAME));
    ASSERT_EQ(m_keyring->getCredentials().size(), 2);
}

TEST_F(KeyringTest, GetCredential1)
{
    ASSERT_EQ(m_keyring->getCredential("Microsoft"), std::nullopt);
}

TEST_F(KeyringTest, DeleteCredential1)
{
    ASSERT_TRUE(m_keyring->deleteCredential("YouTube"));
    ASSERT_EQ(m_keyring->getCredentials().size(), 1);
    ASSERT_EQ(m_keyring->getCredential("YouTube"), std::nullopt);
}

TEST_F(KeyringTest, DeleteCredential2)
{
    ASSERT_TRUE(m_keyring->deleteCredential("GitHub"));
    ASSERT_EQ(m_keyring->getCredentials().size(), 0);
    ASSERT_EQ(m_keyring->getCredential("GitHub"), std::nullopt);
}

TEST_F(KeyringTest, DeleteCredential3)
{
    ASSERT_FALSE(m_keyring->deleteCredential("YouTube"));
}

TEST_F(KeyringTest, Destroy)
{
    ASSERT_TRUE(m_keyring->destroy());
    ASSERT_EQ(m_keyring->getCredentials().size(), 0);
    ASSERT_FALSE(m_keyring->isSavingToDisk());
    ASSERT_FALSE(std::filesystem::exists(m_keyringPath));
}
#include <gtest/gtest.h>
#include <filesystem>
#include "filesystem/userdirectories.h"
#include "keyring/keyring.h"
#include "system/environment.h"

using namespace Nickvision::Filesystem;
using namespace Nickvision::Keyring;
using namespace Nickvision::System;

class KeyringTest : public testing::Test
{
public:
    static std::string m_keyringName;
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

std::string KeyringTest::m_keyringName{ "org.nickvision.libnick.test" };
std::unique_ptr<Keyring> KeyringTest::m_keyring{ nullptr };
std::filesystem::path KeyringTest::m_keyringPath{ UserDirectories::get(UserDirectory::Config) / "Nickvision" / "Keyring" / "org.nickvision.libnick.test.ring2" };

TEST_F(KeyringTest, Create)
{
    ASSERT_NO_THROW(m_keyring = std::make_unique<Keyring>(m_keyringName));
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
    ASSERT_TRUE(m_keyring->add(credential));
    ASSERT_EQ(m_keyring->getAll().size(), 1);
    ASSERT_EQ(m_keyring->get("YouTube"), credential);
}

TEST_F(KeyringTest, AddCredential2)
{
    Credential credential{ "GitHub", "https://github.com", "theawesomeguy", "abc123!" };
    ASSERT_TRUE(m_keyring->add(credential));
    ASSERT_EQ(m_keyring->getAll().size(), 2);
    ASSERT_EQ(m_keyring->get("GitHub"), credential);
}

TEST_F(KeyringTest, AddCredential3)
{
    Credential credential{ "YouTube", "https://youtube.com", "me@gmail.com", "abc123!" };
    ASSERT_FALSE(m_keyring->add(credential));
}

TEST_F(KeyringTest, UpdateCredential1)
{
    Credential credential{ "YouTube", "https://youtube.com", "me@gmail.com", "abc123!" };
    ASSERT_TRUE(m_keyring->update(credential));
    ASSERT_EQ(m_keyring->getAll().size(), 2);
    ASSERT_EQ(m_keyring->get("YouTube"), credential);
}

TEST_F(KeyringTest, ReloadAndValidate)
{
    ASSERT_NO_THROW(m_keyring.reset());
    ASSERT_NO_THROW(m_keyring = std::make_unique<Keyring>(m_keyringName));
    ASSERT_EQ(m_keyring->getAll().size(), 2);
}

TEST_F(KeyringTest, GetBadCredential1)
{
    ASSERT_EQ(m_keyring->get("Microsoft"), std::nullopt);
}

TEST_F(KeyringTest, RemoveCredential1)
{
    ASSERT_TRUE(m_keyring->remove("YouTube"));
    ASSERT_EQ(m_keyring->getAll().size(), 1);
    ASSERT_EQ(m_keyring->get("YouTube"), std::nullopt);
}

TEST_F(KeyringTest, RemoveCredential2)
{
    ASSERT_TRUE(m_keyring->remove("GitHub"));
    ASSERT_EQ(m_keyring->getAll().size(), 0);
    ASSERT_EQ(m_keyring->get("GitHub"), std::nullopt);
}

TEST_F(KeyringTest, RemoveCredential3)
{
    ASSERT_FALSE(m_keyring->remove("YouTube"));
}

TEST_F(KeyringTest, Destroy)
{
    ASSERT_TRUE(m_keyring->destroy());
    ASSERT_EQ(m_keyring->getAll().size(), 0);
    ASSERT_FALSE(m_keyring->isSavingToDisk());
    ASSERT_FALSE(std::filesystem::exists(m_keyringPath));
}
#include <gtest/gtest.h>
#include "keyring/keyring.h"
#include "keyring/keyringdialogcontroller.h"
#include "system/environment.h"

using namespace Nickvision::Keyring;
using namespace Nickvision::System;

class KeyringTest : public testing::Test
{
public:
    static std::unique_ptr<KeyringDialogController> m_controller;

    static void SetUpTestSuite()
    {
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            return;
        }
#endif
        Keyring::destroy("org.nickvision.aura.test.keyring");
        m_controller = std::make_unique<KeyringDialogController>("org.nickvision.aura.test.keyring", Keyring::access("org.nickvision.aura.test.keyring"));
    }
};

std::unique_ptr<KeyringDialogController> KeyringTest::m_controller = nullptr;

TEST_F(KeyringTest, CheckValidKeyring)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->isEnabled());
    ASSERT_TRUE(m_controller->isValid());
}

TEST_F(KeyringTest, ManageCredentials)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->addCredential({ "YT", "https://youtube.com", "theawesomeguy", "abc123!" }));
    ASSERT_TRUE(m_controller->addCredential({ "Google", "https://google.com", "me@gmail.com", "abc12345!" }));
    ASSERT_TRUE(m_controller->getAllCredentials().size() == 2);
    Credential cred{ m_controller->getAllCredentials()[0] };
    ASSERT_EQ(cred.getName(), "YT");
    ASSERT_EQ(cred.getUri(), "https://youtube.com");
    ASSERT_EQ(cred.getUsername(), "theawesomeguy");
    ASSERT_EQ(cred.getPassword(), "abc123!");
    ASSERT_TRUE(m_controller->deleteCredential(cred.getId()));
    ASSERT_TRUE(m_controller->getAllCredentials().size() == 1);
}

TEST_F(KeyringTest, ValidateCredential1)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->validateCredential({ "YT", "https://youtube.com", "theawesomeguy", "abc123!" }) == CredentialCheckStatus::Valid);
}

TEST_F(KeyringTest, ValidateCredential2)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->validateCredential({ "", "https://youtube.com", "theawesomeguy", "abc123!" }) == CredentialCheckStatus::EmptyName);
}

TEST_F(KeyringTest, ValidateCredential3)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->validateCredential({ "YT", "https://youtube.com", "theawesomeguy", "" }) == CredentialCheckStatus::Valid);
}

TEST_F(KeyringTest, ValidateCredential4)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->validateCredential({ "YT", "https://youtube.com", "", "" }) == CredentialCheckStatus::EmptyUsernamePassword);
}

TEST_F(KeyringTest, DisableKeyring)
{
#ifdef __APPLE__
        if(!Environmment::getEnvVar("GITHUB_ACTIONS").empty())
        {
            GTEST_SKIP();
        }
#endif
    ASSERT_TRUE(m_controller->disableKeyring());
}
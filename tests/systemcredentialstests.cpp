#include <gtest/gtest.h>
#include "keyring/credential.h"
#include "keyring/systemcredentials.h"

using namespace Nickvision::Keyring;

static const std::string randCredentialName{ "RandomAuraTestCredential1" };

TEST(SystemCredentialsTests, AddRandCredential)
{
    std::optional<Credential> cred{ SystemCredentials::addCredential(randCredentialName) };
    ASSERT_TRUE(cred.has_value());
    ASSERT_TRUE(cred.value().getName() == randCredentialName);
    ASSERT_TRUE(!cred.value().getPassword().empty());
}

TEST(SystemCredentialsTests, FetchRandCredential)
{
    std::optional<Credential> cred{ SystemCredentials::getCredential(randCredentialName) };
    ASSERT_TRUE(cred.has_value());
    ASSERT_TRUE(cred.value().getName() == randCredentialName);
    ASSERT_TRUE(!cred.value().getPassword().empty());
}

TEST(SystemCredentialsTests, UpdateRandCredential)
{
    Credential updatedCred{ randCredentialName, "", "", "abc123" };
    ASSERT_TRUE(SystemCredentials::updateCredential(updatedCred));
    std::optional<Credential> cred{ SystemCredentials::getCredential(randCredentialName) };
    ASSERT_TRUE(cred.has_value());
    ASSERT_TRUE(cred.value().getName() == randCredentialName);
    ASSERT_TRUE(cred.value().getPassword() == "abc123");
}

TEST(SystemCredentialsTest, DeleteRandCredential)
{
    ASSERT_TRUE(SystemCredentials::deleteCredential(randCredentialName));
}
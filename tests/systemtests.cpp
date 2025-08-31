#include <gtest/gtest.h>
#include <memory>
#include "system/credentials.h"
#include "system/environment.h"
#include "system/suspendinhibitor.h"

using namespace Nickvision::Keyring;
using namespace Nickvision::System;

class SystemTest : public testing::Test
{
public:
    static std::string m_randCredentialName;
    static std::unique_ptr<SuspendInhibitor> m_inhibitor;

    static void SetUpTestSuite()
    {
        m_inhibitor = std::make_unique<SuspendInhibitor>();
    }
};

std::string SystemTest::m_randCredentialName = "RandomAuraTestCredential1";
std::unique_ptr<SuspendInhibitor> SystemTest::m_inhibitor = nullptr;

TEST_F(SystemTest, GetRandomCredential)
{
    ASSERT_EQ(Credentials::get(m_randCredentialName), std::nullopt);
}

TEST_F(SystemTest, CreateRandomCredential)
{
    std::optional<Credential> cred{ Credentials::create(m_randCredentialName) };
    ASSERT_TRUE(cred.has_value());
    ASSERT_EQ(cred.value().getName(), m_randCredentialName);
    ASSERT_TRUE(!cred.value().getPassword().empty());
}

TEST_F(SystemTest, FetchRandCredential)
{
    std::optional<Credential> cred{ Credentials::get(m_randCredentialName) };
    ASSERT_TRUE(cred.has_value());
    ASSERT_EQ(cred.value().getName(), m_randCredentialName);
    ASSERT_TRUE(!cred.value().getPassword().empty());
}

TEST_F(SystemTest, UpdateRandCredential)
{
    Credential updatedCred{ m_randCredentialName, "", "", "abc123" };
    ASSERT_TRUE(Credentials::update(updatedCred));
}

TEST_F(SystemTest, FetchUpdatedRandCredential)
{
    std::optional<Credential> cred{ Credentials::get(m_randCredentialName) };
    ASSERT_TRUE(cred.has_value());
    ASSERT_EQ(cred.value().getName(), m_randCredentialName);
    ASSERT_EQ(cred.value().getPassword(), "abc123");
}

TEST_F(SystemTest, RemoveRandomCredential)
{
    ASSERT_TRUE(Credentials::remove(m_randCredentialName));
}

TEST_F(SystemTest, InhibitSuspend)
{
#ifdef __linux__
    if(Environment::hasVariable("GITHUB_ACTIONS"))
    {
        GTEST_SKIP();
    }
#endif
    ASSERT_TRUE(SystemTest::m_inhibitor->inhibit());
    ASSERT_TRUE(SystemTest::m_inhibitor->isInhibiting());
}

TEST_F(SystemTest, UninhibitSuspend)
{
#ifdef __linux__
    if(Environment::hasVariable("GITHUB_ACTIONS"))
    {
        GTEST_SKIP();
    }
#endif
    ASSERT_TRUE(SystemTest::m_inhibitor->uninhibit());
    ASSERT_FALSE(SystemTest::m_inhibitor->isInhibiting());
}

TEST_F(SystemTest, GetPath)
{
    ASSERT_FALSE(Environment::getVariable("PATH").empty());
}

TEST_F(SystemTest, SetTestVar)
{
    ASSERT_TRUE(Environment::setVariable("AURA_TEST_VAR", "yes"));
}

TEST_F(SystemTest, EnsureTestVar)
{
    ASSERT_EQ(Environment::getVariable("AURA_TEST_VAR"), "yes");
}

TEST_F(SystemTest, TestTestVar)
{
    ASSERT_TRUE(Environment::testVariable("AURA_TEST_VAR"));
}

TEST_F(SystemTest, ClearTestVar)
{
    ASSERT_TRUE(Environment::clearVariable("AURA_TEST_VAR"));
}

TEST_F(SystemTest, Exec)
{
#ifdef _WIN32
    ASSERT_EQ(Environment::exec("echo \"Hello World\""), "\"Hello World\"\r\n");
#else
    ASSERT_EQ(Environment::exec("echo \"Hello World\""), "Hello World\n");
#endif
}

TEST_F(SystemTest, RunningInformationChecks)
{
    ASSERT_TRUE(!Environment::getExecutableDirectory().empty());
    OperatingSystem os{ Environment::getOperatingSystem() };
    DeploymentMode mode{ Environment::getDeploymentMode() };
#ifdef _WIN32
    ASSERT_EQ(os, OperatingSystem::Windows);
#elif defined(__linux__)
    ASSERT_EQ(os, OperatingSystem::Linux);
#elif defined(__APPLE__)
    ASSERT_EQ(os, OperatingSystem::MacOS);
#endif
    ASSERT_EQ(mode, DeploymentMode::Local);
}

TEST_F(SystemTest, DependencyCheck)
{
#ifdef _WIN32
    std::filesystem::path globalDependency{ Environment::findDependency("cmd", DependencySearchOption::Global) };
    std::filesystem::path appDependency{ Environment::findDependency("cmd", DependencySearchOption::App) };
    std::filesystem::path systemDependency{ Environment::findDependency("cmd", DependencySearchOption::System) };
    std::filesystem::path localDependency{ Environment::findDependency("cmd", DependencySearchOption::Local) };
#else
    std::filesystem::path globalDependency{ Environment::findDependency("ls", DependencySearchOption::Global) };
    std::filesystem::path appDependency{ Environment::findDependency("ls", DependencySearchOption::App) };
    std::filesystem::path systemDependency{ Environment::findDependency("ls", DependencySearchOption::System) };
    std::filesystem::path localDependency{ Environment::findDependency("ls", DependencySearchOption::Local) };
#endif
    ASSERT_TRUE(!globalDependency.empty());
    ASSERT_TRUE(std::filesystem::exists(globalDependency));
    ASSERT_TRUE(appDependency.empty());
    ASSERT_TRUE(!std::filesystem::exists(appDependency));
    ASSERT_TRUE(!systemDependency.empty());
    ASSERT_TRUE(std::filesystem::exists(systemDependency));
    ASSERT_TRUE(localDependency.empty());
    ASSERT_TRUE(!std::filesystem::exists(localDependency));
}

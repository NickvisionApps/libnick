#include <gtest/gtest.h>
#include "system/environment.h"
#include "update/updater.h"
#include "update/version.h"

using namespace Nickvision::System;
using namespace Nickvision::Update;

TEST(UpdaterTests, ParabolicStableUpdate)
{
    Updater updater{ "https://github.com/NickvisionApps/Parabolic" };
    Version stable{ updater.fetchCurrentVersion(VersionType::Stable) };
    ASSERT_TRUE(!stable.empty());
    ASSERT_TRUE(stable.getVersionType() == VersionType::Stable);
    ASSERT_TRUE(stable >= Version("2023.12.0"));
}

#ifdef _WIN32
TEST(UpdaterTests, SpotlightWindowsUpdate)
{
    if(!Environment::getVariable("GITHUB_ACTIONS").empty())
    {
        GTEST_SKIP() << "Skipping Windows update test on GitHub Actions";
    }
    Updater updater{ "https://github.com/NickvisionApps/Spotlight" };
    Version stable{ updater.fetchCurrentVersion(VersionType::Stable) };
    ASSERT_TRUE(!stable.empty());
    ASSERT_TRUE(updater.windowsUpdate(VersionType::Stable));
}
#endif
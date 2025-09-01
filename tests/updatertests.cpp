#include <gtest/gtest.h>
#include "system/environment.h"
#include "update/updater.h"
#include "update/version.h"

using namespace Nickvision::System;
using namespace Nickvision::Update;

TEST(UpdaterTests, ParabolicStableUpdate)
{
    if(Environment::hasVariable("GITHUB_ACTIONS"))
    {
        GTEST_SKIP();
    }
    Updater updater{ "https://github.com/NickvisionApps/Parabolic" };
    Version stable{ updater.fetchCurrentVersion(VersionType::Stable) };
    ASSERT_TRUE(!stable.empty());
    ASSERT_TRUE(stable.getVersionType() == VersionType::Stable);
    ASSERT_TRUE(stable >= Version("2025.8.0"));
}

TEST(UpdaterTests, YtdlpUpdate)
{
    if(Environment::hasVariable("GITHUB_ACTIONS"))
    {
        GTEST_SKIP();
    }
    std::filesystem::path downloadPath{ Environment::getExecutableDirectory() / "yt-dlp" };
    Updater updater{ "https://github.com/yt-dlp/yt-dlp" };
    Version stable{ updater.fetchCurrentVersion(VersionType::Stable) };
    ASSERT_TRUE(!stable.empty());
    ASSERT_TRUE(stable.getVersionType() == VersionType::Stable);
    ASSERT_TRUE(stable >= Version("2025.8.11"));
    ASSERT_TRUE(updater.downloadUpdate(VersionType::Stable, downloadPath, "yt-dlp"));
    ASSERT_TRUE(std::filesystem::exists(downloadPath));
    ASSERT_TRUE(std::filesystem::remove(downloadPath));
}

#ifdef _WIN32
TEST(UpdaterTests, ParabolicWindowsUpdate)
{
    if(Environment::hasVariable("GITHUB_ACTIONS"))
    {
        GTEST_SKIP();
    }
    Updater updater{ "https://github.com/NickvisionApps/Parabolic" };
    Version stable{ updater.fetchCurrentVersion(VersionType::Stable) };
    ASSERT_TRUE(!stable.empty());
    ASSERT_TRUE(updater.windowsUpdate(VersionType::Stable));
}
#endif

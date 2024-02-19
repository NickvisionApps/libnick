#include <gtest/gtest.h>
#include "update/updater.h"
#include "update/version.h"

using namespace Nickvision::Update;

TEST(UpdaterTests, ParabolicStableUpdate)
{
    Updater updater{ "https://github.com/NickvisionApps/Parabolic" };
    Version stable{ updater.fetchCurrentStableVersion() };
    ASSERT_TRUE(!stable.empty());
    ASSERT_TRUE(stable.getVersionType() == VersionType::Stable);
    ASSERT_TRUE(stable >= Version("2023.12.0"));
}
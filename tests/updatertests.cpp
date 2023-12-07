#include <gtest/gtest.h>
#include "version.h"
#include "update/updater.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Update;

TEST(UpdaterTests, ParabolicStableUpdate)
{
	Updater updater{ "https://github.com/NickvisionApps/Parabolic" };
	Version stable{ updater.fetchCurrentStableVersion() };
	std::cout << stable << std::endl;
	EXPECT_TRUE(!stable.empty());
	EXPECT_TRUE(stable.getVersionType() == VersionType::Stable);
	EXPECT_TRUE(stable >= Version("2023.11.1"));
}
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
	ASSERT_TRUE(!stable.empty());
	ASSERT_TRUE(stable.getVersionType() == VersionType::Stable);
	ASSERT_TRUE(stable >= Version("2023.11.1"));
}
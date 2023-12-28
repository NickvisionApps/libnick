#include <gtest/gtest.h>
#include "aura.h"
#include "dependencylocator.h"
#include "userdirectories.h"

using namespace Nickvision::Aura;

enum class Theme
{
	Light = 0,
	Dark,
	System
};

class AppConfig : public ConfigurationBase
{
public:
	AppConfig(const std::string& key) : ConfigurationBase(key) 
	{ 

	}

	Theme getTheme() const
	{
		return (Theme)m_json.get("Theme", (int)Theme::System).asInt();
	}

	void setTheme(Theme theme)
	{
		m_json["Theme"] = (int)theme;
	}
};

TEST(AuraTests, InitAura)
{
	ASSERT_NO_THROW(Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests"));
}

TEST(AuraTests, EnsureAura)
{
	ASSERT_NO_THROW(Aura::getActive());
}

TEST(AuraTests, EnsureDefaultAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::System);
}

TEST(AuraTests, ChangeAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	config.setTheme(Theme::Light);
	ASSERT_TRUE(config.save());
	ASSERT_EQ(config.getTheme(), Theme::Light);
}

TEST(AuraTests, EnsureChangeInAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::Light);
}

TEST(AuraTests, ResetAppConfig)
{
	ASSERT_TRUE(std::filesystem::remove(UserDirectories::getApplicationConfig() / ("config.json")));
}

TEST(AuraTests, DependencyCheck)
{
#ifdef _WIN32
	std::filesystem::path dependency{ DependencyLocator::find("cmd") };
#elif defined(__linux__)
	std::filesystem::path dependency{ DependencyLocator::find("ls") };
#endif
	ASSERT_TRUE(!dependency.empty());
	ASSERT_TRUE(std::filesystem::exists(dependency));
}
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

class AuraTest : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests");
	}
};

TEST_F(AuraTest, EnsureAura)
{
	ASSERT_NO_THROW(Aura::getActive());
}

TEST_F(AuraTest, EnsureDefaultAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::System);
}

TEST_F(AuraTest, ChangeAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	config.setTheme(Theme::Light);
	ASSERT_TRUE(config.save());
	ASSERT_EQ(config.getTheme(), Theme::Light);
}

TEST_F(AuraTest, EnsureChangeInAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::Light);
}

TEST_F(AuraTest, ResetAppConfig)
{
	ASSERT_TRUE(std::filesystem::remove(UserDirectories::getApplicationConfig() / ("config.json")));
}

#ifdef _WIN32
TEST_F(AuraTest, DependencyCheckCmd)
{
	ASSERT_FALSE(DependencyLocator::find("cmd").empty());
}
#endif

#ifndef _WIN32
TEST_F(AuraTest, DependencyCheckLs)
{
	ASSERT_FALSE(DependencyLocator::find("ls").empty());
}
#endif
#include <gtest/gtest.h>
#include "aura.h"

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
	ASSERT_EQ(config.getTheme(), Theme::Light);
	ASSERT_NO_THROW(config.save());
}

TEST_F(AuraTest, EnsureChangeInAppConfig)
{
	AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
	ASSERT_EQ(config.getTheme(), Theme::Light);
}
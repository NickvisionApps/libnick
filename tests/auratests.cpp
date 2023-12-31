#include <gtest/gtest.h>
#include "aura.h"
#include "dependencylocator.h"
#include "userdirectories.h"
#include "notifications/shellnotification.h"

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
		Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");
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

TEST_F(AuraTest, DependencyCheck)
{
#ifdef _WIN32
	std::filesystem::path dependency{ DependencyLocator::find("cmd") };
#elif defined(__linux__)
	std::filesystem::path dependency{ DependencyLocator::find("ls") };
#endif
	ASSERT_TRUE(!dependency.empty());
	ASSERT_TRUE(std::filesystem::exists(dependency));
}

TEST_F(AuraTest, ShellNotification)
{
#ifdef _WIN32
	ASSERT_NO_THROW(Notifications::ShellNotification::show({ "Test", "Hello from Notifications::ShellNotification::show()!", Notifications::NotificationSeverity::Informational }, GetConsoleWindow()));
#elif defined(__linux__)
	ASSERT_NO_THROW(Notifications::ShellNotification::show({ "Test", "Hello from Notifications::ShellNotification::show()!", Notifications::NotificationSeverity::Success }));
#endif
}
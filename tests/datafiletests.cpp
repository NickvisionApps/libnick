#include <gtest/gtest.h>
#include <memory>
#include "app/datafilebase.h"
#include "app/datafilemanager.h"
#include "app/windowgeometry.h"
#include "filesystem/userdirectories.h"

using namespace Nickvision::App;
using namespace Nickvision::Filesystem;

enum class Theme
{
    Light = 0,
    Dark,
    System
};

class AppConfig : public DataFileBase
{
public:
    AppConfig(const std::string& key, const std::string& appName)
        : DataFileBase{ key, appName } 
    { 

    }

    Theme getTheme() const
    {
        return static_cast<Theme>(m_json.get("Theme", (int)Theme::System).asInt());
    }

    void setTheme(Theme theme)
    {
        m_json["Theme"] = static_cast<int>(theme);
    }

    WindowGeometry getWindowGeometry()
    {
        WindowGeometry geometry;
        const Json::Value json{ m_json["WindowGeometry"] };
        geometry.setWidth(json.get("Width", 800).asInt64());
        geometry.setHeight(json.get("Height", 600).asInt64());
        geometry.setIsMaximized(json.get("IsMaximized", false).asBool());
        return geometry;
    }

    void setWindowGeometry(const WindowGeometry& geometry)
    {
        m_json["WindowGeometry"]["Width"] = static_cast<Json::Int64>(geometry.getWidth());
        m_json["WindowGeometry"]["Height"] = static_cast<Json::Int64>(geometry.getHeight());
        m_json["WindowGeometry"]["IsMaximized"] = geometry.isMaximized();
    }

    bool getAutomaticallyCheckForUpdates() const
    {
        return m_json.get("AutomaticallyCheckForUpdates", true).asBool();
    }

    void setAutomaticallyCheckForUpdates(bool value)
    {
        m_json["AutomaticallyCheckForUpdates"] = value;
    }
};

class DataFileTest : public testing::Test
{
public:
    static std::shared_ptr<DataFileManager> m_manager;

    static void SetUpTestSuite()
    {
        std::filesystem::remove(UserDirectories::get(ApplicationUserDirectory::Config, "Nickvision Aura Tests") / ("config.json"));
        m_manager = std::make_shared<DataFileManager>("Nickvision Aura Tests");
    }
};

std::shared_ptr<DataFileManager> DataFileTest::m_manager{ nullptr };

TEST_F(DataFileTest, EnsureDefaultAppConfig)
{
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(config.getTheme(), Theme::System);
    ASSERT_EQ(geometry.getWidth(), 800);
    ASSERT_EQ(geometry.getHeight(), 600);
    ASSERT_EQ(geometry.isMaximized(), false);
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), true);
}

TEST_F(DataFileTest, ChangeAppConfig1)
{
    AppConfig& config{m_manager->get<AppConfig>("config") };
    ASSERT_NO_THROW(config.setTheme(Theme::Light));
    ASSERT_NO_THROW(config.setWindowGeometry(WindowGeometry{ 1920, 1080, true }));
    ASSERT_TRUE(config.save());
}

TEST_F(DataFileTest, EnsureChangeInAppConfig)
{
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    ASSERT_EQ(config.getTheme(), Theme::Light);
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
}

TEST_F(DataFileTest, ChangeAppConfig2)
{
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    ASSERT_NO_THROW(config.setAutomaticallyCheckForUpdates(false));
    ASSERT_TRUE(config.save());
}

TEST_F(DataFileTest, EnsureChangeInAppConfig2)
{
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), false);
}
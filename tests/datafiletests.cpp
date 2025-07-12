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
    AppConfig(const std::string& key, const std::string& appName, bool isPortable)
        : DataFileBase{ key, appName, isPortable }
    { 

    }

    Theme getTheme() const
    {
        const boost::json::value& theme{ m_json["Theme"] };
        if(!theme.is_int64())
        {
            return Theme::System;
        }
        return static_cast<Theme>(theme.as_int64());
    }

    void setTheme(Theme theme)
    {
        m_json["Theme"] = static_cast<int>(theme);
    }

    WindowGeometry getWindowGeometry() const
    {
        WindowGeometry geometry;
        if(!m_json["WindowGeometry"].is_object())
        {
            geometry.setWidth(800);
            geometry.setHeight(600);
            geometry.setIsMaximized(false);
            return geometry;
        }
        boost::json::object& obj{ m_json["WindowGeometry"].as_object() };
        geometry.setWidth(obj["Width"].is_int64() ? obj["Width"].as_int64() : 800);
        geometry.setHeight(obj["Height"].is_int64() ? obj["Height"].as_int64() : 600);
        geometry.setIsMaximized(obj["IsMaximized"].is_bool() ? obj["IsMaximized"].as_bool() : false);
        return geometry;
    }

    void setWindowGeometry(const WindowGeometry& geometry)
    {
        boost::json::object obj;
        obj["Width"] = geometry.getWidth();
        obj["Height"] = geometry.getHeight();
        obj["IsMaximized"] = geometry.isMaximized();
        m_json["WindowGeometry"] = obj;
    }

    bool getAutomaticallyCheckForUpdates() const
    {
        const boost::json::value& value{ m_json["AutomaticallyCheckForUpdates"] };
        if(!value.is_bool())
        {
            return true;
        }
        return value.as_bool();
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
    static std::shared_ptr<DataFileManager> m_portableManager;

    static void SetUpTestSuite()
    {
        std::filesystem::remove(UserDirectories::get(ApplicationUserDirectory::Config, "Nickvision Aura Tests") / ("config.json"));
        std::filesystem::remove("config.json");
        m_manager = std::make_shared<DataFileManager>("Nickvision Aura Tests", false);
        m_portableManager = std::make_shared<DataFileManager>("Nickvision Aura Tests", true);
    }

    static void TearDownTestSuite()
    {
        m_manager.reset();
        m_portableManager.reset();
        std::filesystem::remove_all(UserDirectories::get(ApplicationUserDirectory::Config, "Nickvision Aura Tests"));
        std::filesystem::remove("config.json");
    }
};

std::shared_ptr<DataFileManager> DataFileTest::m_manager{ nullptr };
std::shared_ptr<DataFileManager> DataFileTest::m_portableManager{ nullptr };

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

TEST_F(DataFileTest, EnsureDefaultPortableAppConfig)
{
    AppConfig& config{ m_portableManager->get<AppConfig>("config") };
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(config.getTheme(), Theme::System);
    ASSERT_EQ(geometry.getWidth(), 800);
    ASSERT_EQ(geometry.getHeight(), 600);
    ASSERT_EQ(geometry.isMaximized(), false);
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), true);
}

TEST_F(DataFileTest, ChangeAppConfig1)
{
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    ASSERT_NO_THROW(config.setTheme(Theme::Light));
    ASSERT_NO_THROW(config.setWindowGeometry(WindowGeometry{ 1920, 1080, true }));
    ASSERT_TRUE(config.save());
}

TEST_F(DataFileTest, ChangePortableAppConfig1)
{
    AppConfig& config{ m_portableManager->get<AppConfig>("config") };
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

TEST_F(DataFileTest, EnsureChangeInPortableAppConfig)
{
    AppConfig& config{ m_portableManager->get<AppConfig>("config") };
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

TEST_F(DataFileTest, ChangePortableAppConfig2)
{
    AppConfig& config{ m_portableManager->get<AppConfig>("config") };
    ASSERT_NO_THROW(config.setAutomaticallyCheckForUpdates(false));
    ASSERT_TRUE(config.save());
}

TEST_F(DataFileTest, EnsureChangeInAppConfig2)
{
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), false);
}

TEST_F(DataFileTest, EnsureChangeInPortableAppConfig2)
{
    AppConfig& config{ m_portableManager->get<AppConfig>("config") };
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), false);
}

TEST_F(DataFileTest, ReloadAndCheckConfig)
{
    m_manager.reset();
    m_manager = std::make_shared<DataFileManager>("Nickvision Aura Tests", false);
    AppConfig& config{ m_manager->get<AppConfig>("config") };
    ASSERT_EQ(config.getTheme(), Theme::Light);
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), false);
}

TEST_F(DataFileTest, ReloadAndCheckPortableConfig)
{
    m_portableManager.reset();
    m_portableManager = std::make_shared<DataFileManager>("Nickvision Aura Tests", true);
    AppConfig& config{ m_portableManager->get<AppConfig>("config") };
    ASSERT_EQ(config.getTheme(), Theme::Light);
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
    ASSERT_EQ(config.getAutomaticallyCheckForUpdates(), false);
}

#include <gtest/gtest.h>
#include <filesystem>
#include <memory>
#include "app/jsonfilebase.h"
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

class AppConfig : public JsonFileBase
{
public:
    AppConfig(const std::filesystem::path& path)
        : JsonFileBase{ path }
    {

    }

    Theme getTheme() const
    {
        return static_cast<Theme>(get("Theme", static_cast<int>(Theme::System)));
    }

    void setTheme(Theme theme)
    {
        set("Theme", static_cast<int>(theme));
    }

    WindowGeometry getWindowGeometry() const
    {
        WindowGeometry geometry;
        boost::json::object obj = get<boost::json::object>("WindowGeometry", {});
        if(obj.empty())
        {
            geometry.setWidth(800);
            geometry.setHeight(600);
            geometry.setIsMaximized(false);
            return geometry;
        }
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
        set("WindowGeometry", obj);
    }

    bool getAutomaticallyCheckForUpdates() const
    {
        return get("AutomaticallyCheckForUpdates", true);
    }

    void setAutomaticallyCheckForUpdates(bool value)
    {
        set("AutomaticallyCheckForUpdates", value);
    }
};

class JsonFileTest : public testing::Test
{
public:
    static std::filesystem::path m_path;
    static std::filesystem::path m_portablePath;
    static std::shared_ptr<AppConfig> m_config;
    static std::shared_ptr<AppConfig> m_portableConfig;

    static void SetUpTestSuite()
    {
        std::filesystem::remove(m_path);
        std::filesystem::remove(m_portablePath);
        m_config = std::make_shared<AppConfig>(m_path);
        m_portableConfig = std::make_shared<AppConfig>(m_portablePath);
    }

    static void TearDownTestSuite()
    {
        m_config.reset();
        m_portableConfig.reset();
        std::filesystem::remove_all(m_path.parent_path());
        std::filesystem::remove("config.json");
    }
};

std::filesystem::path JsonFileTest::m_path{ UserDirectories::get(ApplicationUserDirectory::Config, "Nickvision Aura Tests") / "config.json" };
std::filesystem::path JsonFileTest::m_portablePath{ "config.json" };
std::shared_ptr<AppConfig> JsonFileTest::m_config{ nullptr };
std::shared_ptr<AppConfig> JsonFileTest::m_portableConfig{ nullptr };

TEST_F(JsonFileTest, EnsureDefaultAppConfig)
{
    WindowGeometry geometry{ m_config->getWindowGeometry() };
    ASSERT_EQ(m_config->getTheme(), Theme::System);
    ASSERT_EQ(geometry.getWidth(), 800);
    ASSERT_EQ(geometry.getHeight(), 600);
    ASSERT_EQ(geometry.isMaximized(), false);
    ASSERT_EQ(m_config->getAutomaticallyCheckForUpdates(), true);
}

TEST_F(JsonFileTest, EnsureDefaultPortableAppConfig)
{
    WindowGeometry geometry{ m_portableConfig->getWindowGeometry() };
    ASSERT_EQ(m_portableConfig->getTheme(), Theme::System);
    ASSERT_EQ(geometry.getWidth(), 800);
    ASSERT_EQ(geometry.getHeight(), 600);
    ASSERT_EQ(geometry.isMaximized(), false);
    ASSERT_EQ(m_portableConfig->getAutomaticallyCheckForUpdates(), true);
}

TEST_F(JsonFileTest, ChangeAppConfig1)
{
    ASSERT_NO_THROW(m_config->setTheme(Theme::Light));
    ASSERT_NO_THROW(m_config->setWindowGeometry(WindowGeometry{ 1920, 1080, true }));
    ASSERT_TRUE(m_config->save());
}

TEST_F(JsonFileTest, ChangePortableAppConfig1)
{
    ASSERT_NO_THROW(m_portableConfig->setTheme(Theme::Light));
    ASSERT_NO_THROW(m_portableConfig->setWindowGeometry(WindowGeometry{ 1920, 1080, true }));
    ASSERT_TRUE(m_portableConfig->save());
}

TEST_F(JsonFileTest, EnsureChangeInAppConfig)
{
    ASSERT_EQ(m_config->getTheme(), Theme::Light);
    WindowGeometry geometry{ m_config->getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
}

TEST_F(JsonFileTest, EnsureChangeInPortableAppConfig)
{
    ASSERT_EQ(m_portableConfig->getTheme(), Theme::Light);
    WindowGeometry geometry{ m_portableConfig->getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
}

TEST_F(JsonFileTest, ChangeAppConfig2)
{
    ASSERT_NO_THROW(m_config->setAutomaticallyCheckForUpdates(false));
    ASSERT_TRUE(m_config->save());
}

TEST_F(JsonFileTest, ChangePortableAppConfig2)
{
    ASSERT_NO_THROW(m_portableConfig->setAutomaticallyCheckForUpdates(false));
    ASSERT_TRUE(m_portableConfig->save());
}

TEST_F(JsonFileTest, EnsureChangeInAppConfig2)
{
    ASSERT_EQ(m_config->getAutomaticallyCheckForUpdates(), false);
}

TEST_F(JsonFileTest, EnsureChangeInPortableAppConfig2)
{
    ASSERT_EQ(m_portableConfig->getAutomaticallyCheckForUpdates(), false);
}

TEST_F(JsonFileTest, ReloadAndCheckConfig)
{
    m_config.reset();
    m_config = std::make_shared<AppConfig>(m_path);
    ASSERT_EQ(m_config->getTheme(), Theme::Light);
    WindowGeometry geometry{ m_config->getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
    ASSERT_EQ(m_config->getAutomaticallyCheckForUpdates(), false);
}

TEST_F(JsonFileTest, ReloadAndCheckPortableConfig)
{
    m_portableConfig.reset();
    m_portableConfig = std::make_shared<AppConfig>(m_portablePath);
    ASSERT_EQ(m_portableConfig->getTheme(), Theme::Light);
    WindowGeometry geometry{ m_portableConfig->getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
    ASSERT_EQ(m_portableConfig->getAutomaticallyCheckForUpdates(), false);
}
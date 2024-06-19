#include <gtest/gtest.h>
#include <thread>
#include "app/aura.h"
#include "app/configurationbase.h"
#include "app/windowgeometry.h"
#include "filesystem/userdirectories.h"
#include "notifications/notifyicon.h"
#include "notifications/notifyiconmenu.h"
#include "notifications/shellnotification.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Notifications;
using namespace Nickvision::System;

enum class Theme
{
    Light = 0,
    Dark,
    System
};

class AppConfig : public ConfigurationBase
{
public:
    AppConfig(const std::string& key)
        : ConfigurationBase{ key } 
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
};

class AuraTest : public testing::Test
{
public:
    static void SetUpTestSuite()
    {
        std::filesystem::remove(UserDirectories::getApplicationConfig() / ("config.json"));
    }
};

TEST_F(AuraTest, EnsureAura)
{
    ASSERT_NO_THROW(Aura::getActive());
}

TEST_F(AuraTest, SetAppInfo)
{
    ASSERT_NO_THROW(Aura::getActive().getAppInfo().setChangelog("* Initial Release"));
    ASSERT_TRUE(!Aura::getActive().getAppInfo().getHtmlChangelog().empty());
}

TEST_F(AuraTest, EnsureDefaultAppConfig)
{
    AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(config.getTheme(), Theme::System);
    ASSERT_EQ(geometry.getWidth(), 800);
    ASSERT_EQ(geometry.getHeight(), 600);
    ASSERT_EQ(geometry.isMaximized(), false);
}

TEST_F(AuraTest, ChangeAppConfig)
{
    AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
    config.setTheme(Theme::Light);
    config.setWindowGeometry(WindowGeometry{ 1920, 1080, true });
    ASSERT_TRUE(config.save());
}

TEST_F(AuraTest, EnsureChangeInAppConfig)
{
    AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
    ASSERT_EQ(config.getTheme(), Theme::Light);
    WindowGeometry geometry{ config.getWindowGeometry() };
    ASSERT_EQ(geometry.getWidth(), 1920);
    ASSERT_EQ(geometry.getHeight(), 1080);
    ASSERT_EQ(geometry.isMaximized(), true);
}

TEST_F(AuraTest, DependencyCheck)
{
#ifdef _WIN32
    std::filesystem::path dependency{ Aura::getActive().findDependency("cmd") };
#else
    std::filesystem::path dependency{ Aura::getActive().findDependency("ls") };
#endif
    ASSERT_TRUE(!dependency.empty());
    ASSERT_TRUE(std::filesystem::exists(dependency));
    ShellNotificationSentEventArgs args{ "Dependency Found!", dependency.string(), NotificationSeverity::Success, "open", dependency.string() };
#ifdef _WIN32
    if (GetConsoleWindow())
    {
        ASSERT_NO_THROW(ShellNotification::send(args, GetConsoleWindow()));
    }
#elif defined(__linux__)
    ASSERT_NO_THROW(ShellNotification::send(args, "Open"));
#elif defined(__APPLE__)
    ASSERT_NO_THROW(ShellNotification::send(args));
#endif
}

TEST_F(AuraTest, RunningInformationChecks)
{
    ASSERT_TRUE(!Aura::getActive().getExecutableDirectory().empty());
#ifdef _WIN32
    ASSERT_TRUE(Aura::getActive().isRunningOnWindows());
#elif defined(__linux__)
    ASSERT_TRUE(Aura::getActive().isRunningOnLinux());
#elif defined(__APPLE__)
    ASSERT_TRUE(Aura::getActive().isRunningOnMac());
#endif
    ASSERT_TRUE(Aura::getActive().isRunningViaLocal());
}

TEST_F(AuraTest, HelpUrlChecks)
{
#ifndef __linux__
    ASSERT_EQ(Aura::getActive().getHelpUrl("index"), "https://htmlpreview.github.io/?https://github.com/NickvisionApps/Denaro/blob/main/NickvisionMoney.Shared/Docs/html/C/index.html");
#else
    ASSERT_EQ(Aura::getActive().getHelpUrl("index"), "help:aura/index");
#endif
}

#ifdef _WIN32
TEST_F(AuraTest, NotifyIconContextMenu)
{
    if (GetConsoleWindow())
    {
        bool waiting{ true };
        NotifyIconMenu contextMenu;
        contextMenu.addAction("Continue", [&waiting]()
        {
            waiting = false;
        });
        contextMenu.addSeparator();
        contextMenu.addAction("PLACEHOLDER", [](){});
        NotifyIcon& notifyIcon{ Aura::getActive().getNotifyIcon(GetConsoleWindow()) };
        ASSERT_TRUE(notifyIcon.show());
        ASSERT_TRUE(notifyIcon.setContextMenu(contextMenu));
        std::cout << "Click \"Continue\" via the context menu of the NotifyIcon to continue..." << std::endl;
        while (waiting)
        {
            MSG msg = { };
            if(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    ASSERT_TRUE(true);
}

TEST_F(AuraTest, NotifyIconNotify)
{
    if (GetConsoleWindow())
    {
        NotifyIcon& notifyIcon{ Aura::getActive().getNotifyIcon(GetConsoleWindow()) };
        ASSERT_NO_THROW(notifyIcon.notify({ "Test", "Hello from Notifications::NotifyIcon::notify()!", NotificationSeverity::Success }));
    }
    ASSERT_TRUE(true);
}
#endif

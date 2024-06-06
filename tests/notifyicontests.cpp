#ifdef _WIN32
#include <gtest/gtest.h>
#include <cstdlib>
#include <memory>
#include "app/aura.h"
#include "notifications/notifyicon.h"
#include "notifications/notifyiconmenu.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::Notifications;
using namespace Nickvision::System;

class NotifyIconTest : public testing::Test
{
public:
    static std::unique_ptr<NotifyIcon> m_notifyIcon;

    static void SetUpTestSuite()
    {
        Aura::getActive().init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");
    }
};

std::unique_ptr<NotifyIcon> NotifyIconTest::m_notifyIcon = nullptr;

TEST_F(NotifyIconTest, CreateIcon)
{
    if (Environment::getVariable("GITHUB_ACTIONS").empty())
    {
        bool waiting{ true };
        NotifyIconMenu contextMenu;
        contextMenu.addAction("Continue", [&waiting]()
        {
                waiting = false;
        });
        contextMenu.addSeparator();
        contextMenu.addAction("PLACEHOLDER", [](){});
        ASSERT_NO_THROW(m_notifyIcon = std::make_unique<NotifyIcon>(GetConsoleWindow(), contextMenu));
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

TEST_F(NotifyIconTest, ShowShellNotification)
{
    if (m_notifyIcon)
    {
        ASSERT_NO_THROW(m_notifyIcon->notify({ "Test", "Hello from Notifications::NotifyIcon::showShellNotification()!", NotificationSeverity::Success }));
    }
    ASSERT_TRUE(true);
}

#endif
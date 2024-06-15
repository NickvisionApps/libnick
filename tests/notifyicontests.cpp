#ifdef _WIN32
#include <gtest/gtest.h>
#include <thread>
#include "app/aura.h"
#include "notifications/notifyicon.h"
#include "notifications/notifyiconmenu.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::Notifications;
using namespace Nickvision::System;

TEST(NotifyIconTests, ContextMenu)
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

TEST(NotifyIconTests, Notify)
{
    if (GetConsoleWindow())
    {
        NotifyIcon& notifyIcon{ Aura::getActive().getNotifyIcon(GetConsoleWindow()) };
        ASSERT_NO_THROW(notifyIcon.notify({ "Test", "Hello from Notifications::NotifyIcon::notify()!", NotificationSeverity::Success }));
    }
    ASSERT_TRUE(true);
}

#endif
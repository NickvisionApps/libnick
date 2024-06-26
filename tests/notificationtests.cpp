#include <gtest/gtest.h>
#include "notifications/shellnotification.h"
#ifdef _WIN32
#include "notifications/notifyicon.h"
#endif

using namespace Nickvision::Notifications;

TEST(NotificationTests, SendShellNotification)
{
    ShellNotificationSentEventArgs e{ "Test", "Hello!", NotificationSeverity::Success };
#ifdef _WIN32
    if (GetConsoleWindow())
    {
        ASSERT_NO_THROW(ShellNotification::send(e, GetConsoleWindow()));
    }
#elif defined(__linux__)
    ASSERT_NO_THROW(ShellNotification::send(e, "org.nickvision.aura.test", "Open"));
#elif defined(__APPLE__)
    ASSERT_NO_THROW(ShellNotification::send(e));
#endif
    ASSERT_TRUE(true);
}

#ifdef _WIN32
TEST(NotificationTests, NotifyIconContextMenu)
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
        NotifyIcon notifyIcon{ GetConsoleWindow(), L"libnick" };
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

TEST(AuraTests, NotifyIconNotify)
{
    if (GetConsoleWindow())
    {
        NotifyIcon notifyIcon{ GetConsoleWindow(), L"libnick" };
        ASSERT_TRUE(notifyIcon.notify({ "Test", "Hello from Notifications::NotifyIcon::notify()!", NotificationSeverity::Success }));
    }
    ASSERT_TRUE(true);
}
#endif
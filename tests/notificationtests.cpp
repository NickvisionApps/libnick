#include <gtest/gtest.h>
#include "filesystem/userdirectories.h"
#include "notifications/shellnotification.h"
#ifdef _WIN32
#include "notifications/notifyicon.h"
#endif

using namespace Nickvision::App;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Notifications;

TEST(NotificationTests, SendShellNotification)
{
    AppInfo info{ "org.nickvision.aura.test", "Nickvision Aura Tests", "libnick_test" };
    ShellNotificationSentEventArgs e{ "Test", "Hello!", NotificationSeverity::Success, "open", UserDirectories::get(UserDirectory::Downloads).string() };
    ASSERT_NO_THROW(ShellNotification::send(e, info, "Open"));
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
#endif

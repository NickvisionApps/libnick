#include <gtest/gtest.h>
#include "filesystem/userdirectories.h"
#include "notifications/shellnotification.h"

using namespace Nickvision::App;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Notifications;

TEST(NotificationTests, SendShellNotification)
{
    AppInfo info{ "org.nickvision.aura.test", "Nickvision Aura Tests", "libnick_test" };
    ShellNotificationSentEventArgs e{ "Test", "Hello!", NotificationSeverity::Success, "open", UserDirectories::get(UserDirectory::Downloads).string() };
    ASSERT_NO_THROW(ShellNotification::send(e, info, "Open"));
}

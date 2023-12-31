#ifdef _WIN32
#include <gtest/gtest.h>
#include <memory>
#include "aura.h"
#include "notifications/notifyicon.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Notifications;

class NotifyIconTest : public testing::Test
{
public:
	static std::unique_ptr<NotifyIcon> m_notifyIcon;

	static void SetUpTestSuite()
	{
		Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");
		m_notifyIcon = std::make_unique<NotifyIcon>(GetConsoleWindow());
	}
};

std::unique_ptr<NotifyIcon> NotifyIconTest::m_notifyIcon = nullptr;

TEST_F(NotifyIconTest, EnsureIcon)
{
	ASSERT_TRUE(m_notifyIcon);
}

TEST_F(NotifyIconTest, ShowShellNotification)
{
	ASSERT_NO_THROW(m_notifyIcon->showShellNotification({ "Test", "Hello from Notifications::NotifyIcon::showShellNotification()!", NotificationSeverity::Success }));
}

#endif
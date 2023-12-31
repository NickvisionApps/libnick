#ifdef _WIN32
#include <gtest/gtest.h>
#include <cstdlib>
#include <memory>
#include "aura.h"
#include "notifications/notifyicon.h"
#include "notifications/notifyiconmenu.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Notifications;

class NotifyIconTest : public testing::Test
{
public:
	static std::unique_ptr<NotifyIcon> m_notifyIcon;

	static void SetUpTestSuite()
	{
		Aura::init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");

	}
};

std::unique_ptr<NotifyIcon> NotifyIconTest::m_notifyIcon = nullptr;

TEST_F(NotifyIconTest, CreateIcon)
{
	if (Aura::getEnvVar("GITHUB_ACTIONS").empty())
	{
		NotifyIconMenu contextMenu;
		contextMenu.addSeparator();
		contextMenu.addAction("Exit", []()
		{
			std::exit(0);
		});
		ASSERT_NO_THROW(m_notifyIcon = std::make_unique<NotifyIcon>(GetConsoleWindow(), contextMenu));
	}
}

TEST_F(NotifyIconTest, ShowShellNotification)
{
	if (m_notifyIcon)
	{
		ASSERT_NO_THROW(m_notifyIcon->showShellNotification({ "Test", "Hello from Notifications::NotifyIcon::showShellNotification()!", NotificationSeverity::Success }));
	}
	ASSERT_TRUE(true);
}

#endif
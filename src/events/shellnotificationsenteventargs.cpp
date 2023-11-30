#include "events/shellnotificationsenteventargs.h"

namespace Nickvision::Aura::Events
{
	ShellNotificationSentEventArgs::ShellNotificationSentEventArgs(const std::string& title, const std::string& message, NotificationSeverity severity, const std::string& action, const std::string& actionParam)
		: NotificationSentEventArgs{ message, severity, action, actionParam },
		m_title{ title }
	{

	}

	const std::string& ShellNotificationSentEventArgs::getTitle() const
	{
		return m_title;
	}
}
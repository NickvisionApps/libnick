#include "notifications/notificationsenteventargs.h"

namespace Nickvision::Aura::Notifications
{
	NotificationSentEventArgs::NotificationSentEventArgs(const std::string& message, NotificationSeverity severity, const std::string& action, const std::string& actionParam)
		: m_message{ message },
		m_severity{ severity },
		m_action{ action },
		m_actionParam{ actionParam }
	{

	}

	const std::string& NotificationSentEventArgs::getMessage() const
	{
		return m_message;
	}

	NotificationSeverity NotificationSentEventArgs::getSeverity() const
	{
		return m_severity;
	}

	const std::string& NotificationSentEventArgs::getAction() const
	{
		return m_action;
	}

	const std::string& NotificationSentEventArgs::getActionParam() const
	{
		return m_actionParam;
	}
}
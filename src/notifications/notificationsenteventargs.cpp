#include "notifications/notificationsenteventargs.h"

namespace Nickvision::Notifications
{
	NotificationSentEventArgs::NotificationSentEventArgs(const std::string& message, NotificationSeverity severity, const std::string& action, const std::string& actionParam) noexcept
		: m_message{ message },
		m_severity{ severity },
		m_action{ action },
		m_actionParam{ actionParam }
	{

	}

	const std::string& NotificationSentEventArgs::getMessage() const noexcept
	{
		return m_message;
	}

	NotificationSeverity NotificationSentEventArgs::getSeverity() const noexcept
	{
		return m_severity;
	}

	const std::string& NotificationSentEventArgs::getAction() const noexcept
	{
		return m_action;
	}

	const std::string& NotificationSentEventArgs::getActionParam() const noexcept
	{
		return m_actionParam;
	}
}
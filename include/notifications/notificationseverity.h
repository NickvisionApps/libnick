#ifndef NOTIFICATIONSEVERITY_H
#define NOTIFICATIONSEVERITY_H

namespace Nickvision::Notifications
{
	/**
	 * @brief Severities for a notification.
	 */
	enum class NotificationSeverity
	{
		Informational = 0,
		Success,
		Warning,
		Error
	};
}

#endif //NOTIFICATIONSEVERITY_H
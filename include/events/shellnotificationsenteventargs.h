#ifndef SHELLNOTIFICATIONSENTEVENTARGS_H
#define SHELLNOTIFICATIONSENTEVENTARGS_H

#include "notificationsenteventargs.h"

namespace Nickvision::Aura::Events
{
	/**
	 * @brief Event args for when a shell notification is sent
	 */
	class ShellNotificationSentEventArgs : public NotificationSentEventArgs
	{
	public:
		/**
		 * @brief Constructs a ShellNotificationSentEventArgs.
		 * @param title The title of the notification
		 * @param message The message of the notification
		 * @param severity The severity of the notification
		 * @param action An additional action for the notification
		 * @param actionParam The parameter of the additional action
		 */
		ShellNotificationSentEventArgs(const std::string& title, const std::string& message, NotificationSeverity severity, const std::string& action = "", const std::string& actionParam = "");
		/**
		 * @brief Gets the title of the notification.
		 * @return The title of the notification
		 */
		const std::string& getTitle() const;
	protected:
		std::string m_title;
	};
}

#endif //SHELLNOTIFICATIONSENTEVENTARGS_H
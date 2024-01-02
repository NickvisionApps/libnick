#ifndef SHELLNOTIFICATION_H
#define SHELLNOTIFICATION_H

#include "shellnotificationsenteventargs.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::Aura::Notifications::ShellNotification
{
#ifdef _WIN32
	/**
	 * @brief Sends a notification to the desktop.
	 * @brief Uses Nickvision::Aura::Notifications::NotifyIcon.
	 * @brief Supports the action "open" with action param being a path of a file or folder to open.
	 * @param e ShellNotificationSentEventArgs
	 * @param hwnd The HWND handle of the main application window. This parameter is only used once on the initial creation of the static NotifyIcon and then is ignored on future calls
	 * @throw std::logic_error Thrown if Aura::init() was not yet called
	 */
	void send(const ShellNotificationSentEventArgs& e, HWND hwnd);
#elif defined(__linux__)
	/**
	 * @brief Sends a notification to the shell.
	 * @brief Uses Gio.Notification on Linux.
	 * @brief Supports the action "open" with action param being a path of a file or folder to open. The app must define an "app.open" action to handle this event.
	 * @param e ShellNotificationSentEventArgs
	 * @throw std::logic_error Thrown if Aura::init() was not yet called
	 */
	void send(const ShellNotificationSentEventArgs& e);
#endif
}

#endif //SHELLNOTIFICATION_H
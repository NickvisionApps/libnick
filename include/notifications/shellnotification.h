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
	 * @brief Shows a notification on the desktop shell.
	 * @brief Uses Nickvision::Aura::Notifications::NotifyIcon on Windows and Gio.Notification on Linux.
	 * @param e ShellNotificationSentEventArgs
	 * @param hwnd The HWND handle of the main application window
	 * @throw std::logic_error Thrown if Aura::init() was not yet called
	 */
	void show(const ShellNotificationSentEventArgs& e, HWND hwnd) noexcept;
#elif defined(__linux__)
	/**
	 * @brief Shows a notification on the desktop shell.
	 * @brief Uses Nickvision::Aura::Notifications::NotifyIcon on Windows and Gio.Notification on Linux.
	 * @brief Supports the action "open" with action param being a path of a file or folder to open. The app must define an "app.open" action to handle this event.
	 * @param e ShellNotificationSentEventArgs
	 * @throw std::logic_error Thrown if Aura::init() was not yet called
	 */
	void show(const ShellNotificationSentEventArgs& e);
#endif
}

#endif //SHELLNOTIFICATION_H
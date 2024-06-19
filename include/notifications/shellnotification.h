#ifndef SHELLNOTIFICATION_H
#define SHELLNOTIFICATION_H

#include <string>
#include "shellnotificationsenteventargs.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::Notifications::ShellNotification
{
#ifdef _WIN32
    /**
     * @brief Sends a notification to the desktop.
     * @brief Uses Nickvision::Notifications::NotifyIcon.
     * @brief Supports the action "open" with action param being a path of a file or folder to open.
     * @param e ShellNotificationSentEventArgs
     * @param hwnd The HWND handle of the main application window. This parameter is only used once on the initial creation of the static NotifyIcon and then is ignored on future calls
     */
    void send(const ShellNotificationSentEventArgs& e, HWND hwnd);
#elif defined(__linux__)
    /**
     * @brief Sends a notification to the shell.
     * @brief Uses Gio.Notification on Linux.
     * @brief Supports the action "open" with action param being a path of a file or folder to open. The app must define an "app.open" action to handle this event.
     * @param e ShellNotificationSentEventArgs
     * @param openText Localized text of "Open"
     */
    void send(const ShellNotificationSentEventArgs& e, const std::string& openText);
#elif defined(__APPLE__)
    /**
     * @brief Sends a notification to the shell.
     * @brief Uses NSUserNotification on macOS.
     * @param e ShellNotificationSentEventArgs
     */
    void send(const ShellNotificationSentEventArgs& e);
#endif
}

#endif //SHELLNOTIFICATION_H
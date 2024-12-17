/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Functions for sending desktop shell notifications.
 */

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
     * @brief Supports the action "open" with action param being a path of a file or folder to open.
     * @param e ShellNotificationSentEventArgs
     * @param hwnd The handle to the main application window
     */
    void send(const ShellNotificationSentEventArgs& e, HWND hwnd);
#elif defined(__linux__)
    /**
     * @brief Sends a notification to the shell.
     * @brief Uses Gio.Notification on GTK applications.
     * @brief Uses FreeDesktop Notifications on non-GTK applications.
     * @brief Supports the action "open" with action param being a path of a file or folder to open. The app must define an "app.open" action to handle this event. (GTK applications only)
     * @param e ShellNotificationSentEventArgs
     * @param appId The application's id
     * @param openText Localized text of "Open"
     */
    void send(const ShellNotificationSentEventArgs& e, const std::string& appId, const std::string& openText);
#elif defined(__APPLE__)
    /**
     * @brief Sends a notification to the shell.
     * @param e ShellNotificationSentEventArgs
     */
    void send(const ShellNotificationSentEventArgs& e);
#endif
}

#endif //SHELLNOTIFICATION_H
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
#include "app/appinfo.h"
#include "shellnotificationsenteventargs.h"

namespace Nickvision::Notifications::ShellNotification
{
    /**
     * @brief Sends a notification to the desktop.
     * @brief On Windows, uses toast notifications.
     * @brief On GTK, uses Gio.Notification.
     * @brief On non-GTK, uses FreeDesktop notification spec on Linux or osascript on Apple.
     * @brief Supports the action "open" with action param being a path of a file or folder to open.
     * @brief On GTK, the app must define an "app.open" action to handle this event.
     * @param e ShellNotificationSentEventArgs
     * @param info The application information
     * @param openText Localized text of "Open"
     * @return True if notification sent
     * @return False if notification not sent
     */
    void send(const ShellNotificationSentEventArgs& e, const App::AppInfo& info, const std::string& openText = "") noexcept;
}

#endif //SHELLNOTIFICATION_H

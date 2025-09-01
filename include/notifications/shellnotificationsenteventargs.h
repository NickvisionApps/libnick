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
 * An event argument for when a shell notification is sent.
 */

#ifndef SHELLNOTIFICATIONSENTEVENTARGS_H
#define SHELLNOTIFICATIONSENTEVENTARGS_H

#include "notificationsenteventargs.h"

namespace Nickvision::Notifications
{
    /**
     * @brief An event argument for when a shell notification is sent.
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
        ShellNotificationSentEventArgs(const std::string& title, const std::string& message, NotificationSeverity severity, const std::string& action = "", const std::string& actionParam = "") noexcept;
        /**
         * @brief Gets the title of the notification.
         * @return The title of the notification
         */
        const std::string& getTitle() const noexcept;

    protected:
        std::string m_title;
    };
}

#endif //SHELLNOTIFICATIONSENTEVENTARGS_H
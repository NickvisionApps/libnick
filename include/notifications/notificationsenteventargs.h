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
 * An event argument for when a notification is sent.
 */

#ifndef NOTIFICATIONSENTEVENTARGS_H
#define NOTIFICATIONSENTEVENTARGS_H

#include <string>
#include "notificationseverity.h"
#include "events/eventargs.h"

namespace Nickvision::Notifications
{
    /**
     * @brief An event argument for when a notification is sent.
     */
    class NotificationSentEventArgs : public Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a NotificationSentEventArgs.
         * @param message The message of the notification
         * @param severity The severity of the notification
         * @param action An additional action for the notification
         * @param actionParam The parameter of the additional action
         */
        NotificationSentEventArgs(const std::string& message, NotificationSeverity severity, const std::string& action = "", const std::string& actionParam = "") noexcept;
        /**
         * @brief Gets the message of the notification.
         * @return The message of the notification
         */
        const std::string& getMessage() const noexcept;
        /**
         * @brief Gets the severity of the notification.
         * @return The severity of the notification
         */
        NotificationSeverity getSeverity() const noexcept;
        /**
         * @brief Gets the optional action of the notification.
         * @return The optional action of the notification
         */
        const std::string& getAction() const noexcept;
        /**
         * @brief Gets the parameter of the optional action.
         * @return The parameter of the optional action
         */
        const std::string& getActionParam() const noexcept;

    protected:
        std::string m_message;
        NotificationSeverity m_severity;
        std::string m_action;
        std::string m_actionParam;
    };
}

#endif //NOTIFICATIONSENTEVENTARGS_H
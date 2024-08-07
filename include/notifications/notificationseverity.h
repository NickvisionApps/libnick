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
 * Severities of a notification.
 */

#ifndef NOTIFICATIONSEVERITY_H
#define NOTIFICATIONSEVERITY_H

namespace Nickvision::Notifications
{
    /**
     * @brief Severities of a notification.
     */
    enum class NotificationSeverity
    {
        Informational = 0, ///< An informational notification.
        Success, ///< A success notification.
        Warning, ///< A warning notification.
        Error ///< An error notification.
    };
}

#endif //NOTIFICATIONSEVERITY_H
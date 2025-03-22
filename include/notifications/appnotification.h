#ifndef APPNOTIFICATION_H
#define APPNOTIFICATION_H

#include "events/event.h"
#include "notificationsenteventargs.h"

namespace Nickvision::Notifications::AppNotification
{
    /**
     * @brief Gets the event for when an app notification is sent.
     * @return The app notification sent event
     */
    Events::Event<NotificationSentEventArgs>& sent();
    /**
     * @brief Sends a notification to the app.
     * @param args NotificationSentEventArgs
     */
    void send(const NotificationSentEventArgs& args);
}

#endif //APPNOTIFICATION_H

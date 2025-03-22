#include "notifications/appnotification.h"

using namespace Nickvision::Events;

namespace Nickvision::Notifications
{
    static Event<NotificationSentEventArgs> s_sent;

    Event<NotificationSentEventArgs>& AppNotification::sent()
    {
        return s_sent;
    }

    void AppNotification::send(const NotificationSentEventArgs& args)
    {
        s_sent.invoke(args);
    }
}

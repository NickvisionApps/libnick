#include "notifications/shellnotificationsenteventargs.h"

namespace Nickvision::Notifications
{
    ShellNotificationSentEventArgs::ShellNotificationSentEventArgs(const std::string& title, const std::string& message, NotificationSeverity severity, const std::string& action, const std::string& actionParam) noexcept
        : NotificationSentEventArgs{ message, severity, action, actionParam },
        m_title{ title }
    {

    }

    const std::string& ShellNotificationSentEventArgs::getTitle() const noexcept
    {
        return m_title;
    }
}
#include "notifications/shellnotification.h"
#include <filesystem>
#include <memory>
#include <string>
#include <thread>
#include "system/environment.h"
#ifdef _WIN32
#include "notifications/notifyicon.h"
#include "notifications/notifyiconmenu.h"
#elif defined(__linux__)
#include <gio/gio.h>
#endif

using namespace Nickvision::System;

namespace Nickvision::Notifications
{
#ifdef _WIN32
    void ShellNotification::send(const ShellNotificationSentEventArgs& e, HWND hwnd)
    {
        NotifyIcon* icon{ NotifyIcon::getFromHWND(hwnd) };
        if(icon)
        {
            icon->notify(e);
        }
        else
        {
            std::vector<wchar_t> title(static_cast<size_t>(GetWindowTextLengthW(hwnd)));
            GetWindowTextW(hwnd, title.data(), static_cast<int>(title.size()));
            icon = new NotifyIcon(hwnd, std::wstring(title.data(), title.size()), { }, false);
            icon->notify(e);
            std::thread t{ [icon]()
            { 
                std::this_thread::sleep_for(std::chrono::seconds(5));
                delete icon;
            } };
            t.detach();
        }
    }
#elif defined(__linux__)
    void ShellNotification::send(const ShellNotificationSentEventArgs& e, const std::string& appId, const std::string& openText)
    {
        if (g_application_get_default())
        {
            GNotification* notification{ g_notification_new(e.getTitle().c_str()) };
            GIcon* icon{ nullptr };
            GFile* fileIcon{ nullptr };
            if (!Environment::hasVariable("SNAP"))
            {
                std::string name{ appId + "-symbolic" };
                icon = g_themed_icon_new(name.c_str());
            }
            else
            {
                std::string path{ Environment::getVariable("SNAP") + "/usr/share/icons/hicolor/symbolic/apps/" + appId + "-symbolic.svg"};
                fileIcon = g_file_new_for_path(path.c_str());
                icon = g_file_icon_new(fileIcon);
            }
            g_notification_set_body(notification, e.getMessage().c_str());
            g_notification_set_icon(notification, icon);
            if (e.getSeverity() == NotificationSeverity::Success)
            {
                g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_HIGH);
            }
            else if (e.getSeverity() == NotificationSeverity::Warning || e.getSeverity() == NotificationSeverity::Error)
            {
                g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_URGENT);
            }
            else
            {
                g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_NORMAL);
            }
            if (e.getAction() == "open" && std::filesystem::exists(e.getActionParam()))
            {
                g_notification_add_button_with_target_value(notification, openText.c_str(), "app.open", g_variant_new_string(e.getActionParam().c_str()));
            }
            g_application_send_notification(g_application_get_default(), appId.c_str(), notification);
            if (fileIcon)
            {
                g_object_unref(G_OBJECT(fileIcon));
            }
            g_object_unref(G_OBJECT(icon));
            g_object_unref(G_OBJECT(notification));
        }
    }
#elif defined(__APPLE__)
    void ShellNotification::send(const ShellNotificationSentEventArgs& e)
    {
        Environment::exec("osascript -e 'display notification \"" + e.getMessage() + "\" with title \"" + e.getTitle() + "\"'");
    }
#endif
}
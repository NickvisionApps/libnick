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
                std::this_thread::sleep_for(std::chrono::seconds(10));
                delete icon;
            } };
            t.detach();
        }
    }
#elif defined(__linux__)
    void ShellNotification::send(const ShellNotificationSentEventArgs& e, const std::string& appId, const std::string& openText)
    {
        std::string iconPath;
        if(!Environment::hasVariable("SNAP"))
        {
            iconPath = appId + "-symbolic";
        }
        else
        {
            iconPath = Environment::getVariable("SNAP") + "/usr/share/icons/hicolor/symbolic/apps/" + appId + "-symbolic.svg";
        }
        if (g_application_get_default())
        {
            GNotification* notification{ g_notification_new(e.getTitle().c_str()) };
            GIcon* icon{ nullptr };
            GFile* fileIcon{ nullptr };
            if (!Environment::hasVariable("SNAP"))
            {
                icon = g_themed_icon_new(iconPath.c_str());
            }
            else
            {
                fileIcon = g_file_new_for_path(iconPath.c_str());
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
        else
        {
            GDBusConnection* connection{ g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr) };
            if(connection)
            {
                GVariant* params{ g_variant_new("(susssasa{sv}i)", appId.c_str(), 0, iconPath.c_str(), e.getTitle().c_str(), e.getMessage().c_str(), nullptr, nullptr, -1) };
                GVariant* result{ g_dbus_connection_call_sync(connection, "org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify", params, nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, nullptr) };
                if(result)
                {
                    g_variant_unref(result);
                }
                g_variant_unref(params);
                g_object_unref(G_OBJECT(connection));
            }
        }
    }
#elif defined(__APPLE__)
    void ShellNotification::send(const ShellNotificationSentEventArgs& e)
    {
        Environment::exec("osascript -e 'display notification \"" + e.getMessage() + "\" with title \"" + e.getTitle() + "\"'");
    }
#endif
}
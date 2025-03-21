#include "notifications/shellnotification.h"
#include <filesystem>
#include <memory>
#include <string>
#include <thread>
#include "helpers/stringhelpers.h"
#include "system/environment.h"
#ifdef _WIN32
#include <wintoastlib.h>
#else
#include <gio/gio.h>
#endif

using namespace Nickvision::App;
using namespace Nickvision::Helpers;
using namespace Nickvision::System;

#ifdef _WIN32
using namespace WinToastLib;

class WinToastHandler : public IWinToastHandler
{
public:
    WinToastHandler()
    {

    }

    WinToastHandler(const std::filesystem::path& openPath)
        : m_openPath{ openPath }
    {

    }

    void toastActivated() const override
    {

    }

    void toastActivated(int actionIndex) const override
    {
        //First and only button --> "Open"
        if(actionIndex == 0 && !m_openPath.empty())
        {
            ShellExecuteW(nullptr, L"open", m_openPath.wstring().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
        }
    }

    void toastActivated(const char*) const override
    {

    }

    void toastDismissed(WinToastDismissalReason) const override
    {

    }

    void toastFailed() const override
    {

    }

private:
    std::filesystem::path m_openPath;
};
#endif

namespace Nickvision::Notifications
{
    void ShellNotification::send(const ShellNotificationSentEventArgs& e, const AppInfo& info, const std::string& openText)
    {
#ifdef _WIN32
        WinToast::WinToastError err;
        static bool initialized{ false };
        if(!initialized)
        {
            WinToast::instance()->setAppName(StringHelpers::wstr(info.getEnglishShortName()));
            WinToast::instance()->setAppUserModelId(StringHelpers::wstr(info.getEnglishShortName()));
            initialized = WinToast::instance()->initialize(&err);
        }
        WinToastTemplate tmpl{ WinToastTemplate::Text02 };
        IWinToastHandler* handler{ nullptr };
        tmpl.setTextField(StringHelpers::wstr(e.getTitle()), WinToastTemplate::FirstLine);
        tmpl.setTextField(StringHelpers::wstr(e.getMessage()), WinToastTemplate::SecondLine);
        if(e.getAction() == "open" && std::filesystem::exists(e.getActionParam()))
        {
            tmpl.addAction(StringHelpers::wstr(openText));
            handler = new WinToastHandler(e.getActionParam());
        }
        else
        {
            handler = new WinToastHandler();
        }
        WinToast::instance()->showToast(tmpl, handler);
#else
        std::string iconPath;
        if(!Environment::hasVariable("SNAP"))
        {
            iconPath = info.getId() + "-symbolic";
        }
        else
        {
            iconPath = Environment::getVariable("SNAP") + "/usr/share/icons/hicolor/symbolic/apps/" + info.getId() + "-symbolic.svg";
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
            g_application_send_notification(g_application_get_default(), info.getId().c_str(), notification);
            if (fileIcon)
            {
                g_object_unref(G_OBJECT(fileIcon));
            }
            g_object_unref(G_OBJECT(icon));
            g_object_unref(G_OBJECT(notification));
        }
        else
        {
#ifdef __linux__
            GDBusConnection* connection{ g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr) };
            if(connection)
            {
                GVariant* params{ g_variant_new("(susssasa{sv}i)", info.getId().c_str(), 0, iconPath.c_str(), e.getTitle().c_str(), e.getMessage().c_str(), nullptr, nullptr, -1) };
                GVariant* result{ g_dbus_connection_call_sync(connection, "org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify", params, nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, nullptr) };
                if(result)
                {
                    g_variant_unref(result);
                }
                g_variant_unref(params);
                g_object_unref(G_OBJECT(connection));
            }
#elif defined(__APPLE__)
            Environment::exec("osascript -e 'display notification \"" + e.getMessage() + "\" with title \"" + e.getTitle() + "\" subtitle \"" + info.getEnglishShortName() + "\"'");
#endif
        }
#endif
    }
}

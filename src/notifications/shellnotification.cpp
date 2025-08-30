#include "notifications/shellnotification.h"
#include <filesystem>
#include <thread>
#include "system/environment.h"
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#else
#include <gio/gio.h>
#endif

#ifdef _WIN32
#define WM_TRAYICON WM_APP + 1
#define TRAYICON_ID 1001
#define TIMER_ID 1002
#endif

using namespace Nickvision::App;
using namespace Nickvision::System;

#ifdef _WIN32
static std::filesystem::path s_openPath;

static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if(msg == WM_TRAYICON)
    {
        if(lParam == NIN_BALLOONUSERCLICK)
        {
            if(std::filesystem::exists(s_openPath))
            {
                ShellExecuteA(hwnd, "open", s_openPath.string().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
            }
        }
    }
    else if(msg == WM_TIMER)
    {
        if(wParam == TIMER_ID)
        {
            KillTimer(hwnd, TIMER_ID);
            PostQuitMessage(0);
        }
    }
    else if(msg == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
#endif

namespace Nickvision::Notifications
{
    void ShellNotification::send(const ShellNotificationSentEventArgs& e, const AppInfo& info, const std::string& openText) noexcept
    {
#ifdef _WIN32
        s_openPath = e.getAction() == "open" ? e.getActionParam() : "";
        std::thread worker{ [e, info]()
        {
            std::string className{ "libnick_notification" };
            WNDCLASSA wc{};
            wc.lpfnWndProc = wndProc;
            wc.hInstance = GetModuleHandleA(nullptr);
            wc.lpszClassName = className.c_str();
            RegisterClassA(&wc);
            HWND hwnd{ CreateWindowExA(0, className.c_str(), info.getShortName().c_str(), 0, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandleA(nullptr), nullptr) };
            if(!hwnd)
            {
                return;
            }
            HICON icn;
            ExtractIconExA(Environment::getExecutablePath().string().c_str(), 0, nullptr, &icn, 1);
            NOTIFYICONDATAA nid{};
            nid.cbSize = sizeof(NOTIFYICONDATAA);
            nid.hWnd = hwnd;
            nid.uID = TRAYICON_ID;
            nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP | NIF_MESSAGE;
            nid.uCallbackMessage = WM_TRAYICON;
            nid.hIcon = icn ? icn : LoadIconA(nullptr, IDI_APPLICATION);
            switch(e.getSeverity())
            {
            case NotificationSeverity::Error:
                nid.dwInfoFlags = NIIF_ERROR;
                break;
            case NotificationSeverity::Warning:
                nid.dwInfoFlags = NIIF_WARNING;
                break;
            default:
                nid.dwInfoFlags = NIIF_INFO;
                break;
            }
            strcpy_s(nid.szTip, info.getShortName().c_str());
            strcpy_s(nid.szInfoTitle, e.getTitle().c_str());
            strcpy_s(nid.szInfo, e.getMessage().c_str());
            Shell_NotifyIconA(NIM_ADD, &nid);
            SetTimer(hwnd, TIMER_ID, 5000, nullptr);
            MSG msg;
            while(GetMessageA(&msg, nullptr, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            Shell_NotifyIconA(NIM_DELETE, &nid);
            if(icn)
            {
                DestroyIcon(icn);
            }
        } };
        worker.detach();
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

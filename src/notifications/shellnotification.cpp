#include "notifications/shellnotification.h"
#include <filesystem>
#include <memory>
#include <string>
#include "aura/aura.h"
#ifdef _WIN32
#include "notifications/notifyicon.h"
#include "notifications/notifyiconmenu.h"
#elif defined(__linux__)
#include <gio/gio.h>
#endif

namespace Nickvision::Notifications
{
#ifdef _WIN32
	void ShellNotification::send(const ShellNotificationSentEventArgs& e, HWND hwnd)
	{
		static std::shared_ptr<NotifyIcon> notifyIcon{ std::make_shared<NotifyIcon>(hwnd, NotifyIconMenu(), true)};
		notifyIcon->hide();
		notifyIcon->notify(e);
	}
#elif defined(__linux__)
	void ShellNotification::send(const ShellNotificationSentEventArgs& e, const std::string& openText)
	{
		if (g_application_get_default())
		{
			GNotification* notification{ g_notification_new(e.getTitle().c_str()) };
			GIcon* icon{ nullptr };
			GFile* fileIcon{ nullptr };
			std::string appId{ Aura::Aura::getActive().getAppInfo().getId() };
			if (Aura::Aura::getEnvVar("SNAP").empty())
			{
				std::string name{ appId + "-symbolic" };
				icon = g_themed_icon_new(name.c_str());
			}
			else
			{
				std::string path{ Aura::Aura::getEnvVar("SNAP") + "/usr/share/icons/hicolor/symbolic/apps/" + appId + "-symbolic.svg"};
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
#endif
}
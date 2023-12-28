#include "notifications/shellnotification.h"
#include <string>
#include "aura.h"
#ifdef __linux__
#include <glib.h>
#include <gio/gio.h>
#endif

namespace Nickvision::Aura::Notifications
{
	bool ShellNotification::send(const ShellNotificationSentEventArgs& args)
	{
#ifdef _WIN32

#elif defined(__linux__)
		GNotification* notification{ g_notification_new(args.getTitle().c_str()) };
		g_notification_set_body(notification, args.getMessage().c_str());
		switch (args.getSeverity())
		{
		case NotificationSeverity::Success:
			
			break;
		case NotificationSeverity::Warning:
		case NotificationSeverity::Error:
			g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_URGENT);
			break;
		default:
			g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_NORMAL);
			break;
		}
		std::string appId{ Aura::getActive().getAppInfo().getId() };
		GIcon* icon{ nullptr };
		GFile* snapIconFile{ nullptr };
		if (Aura::getEnvVar("SNAP").empty())
		{
			std::string iconName{ appId + "-symbolic" };
			icon = g_themed_icon_new(iconName.c_str());
		}
		else
		{
			std::string path{ Aura::getEnvVar("SNAP") + "/usr/share/icons/hicolor/symbolic/apps/" + appId + "-symbolic.svg" };
			snapIconFile = g_file_new_for_path(path.c_str());
			icon = g_file_icon_new(snapIconFile);
		}
		g_notification_set_icon(notification, icon);
		g_application_send_notification(g_application_get_default(), appId, notification);
		g_object_unref(G_OBJECT(icon));
		if (snapIconFile)
		{
			g_object_unref(G_OBJECT(snapIconFile));
		}
		g_object_unref(G_OBJECT(notification));
#endif
		return true;
	}
}
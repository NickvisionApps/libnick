#ifdef _WIN32
#include "notifications/notifyicon.h"
#include <stdexcept>
#include <strsafe.h>
#include "aura.h"

namespace Nickvision::Aura::Notifications
{
	NotifyIcon::NotifyIcon(HWND hwnd)
		: m_hwnd{ hwnd }
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (CoCreateGuid(&m_guid) != S_OK)
		{
			throw std::runtime_error("Unable to create guid.");
		}
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		notify.uFlags |= NIF_ICON | NIF_TIP | NIF_SHOWTIP;
		notify.hIcon = (HICON)GetClassLongPtrA(hwnd, GCLP_HICON);
		StringCchCopyA(notify.szTip, ARRAYSIZE(notify.szTip), Aura::getActive().getAppInfo().getName().c_str());
		notify.uVersion = NOTIFYICON_VERSION_4;
		Shell_NotifyIconA(NIM_ADD, &notify);
		Shell_NotifyIconA(NIM_SETVERSION, &notify);
	}

	NotifyIcon::~NotifyIcon() noexcept
	{
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		Shell_NotifyIconA(NIM_DELETE, &notify);
	}

	void NotifyIcon::showShellNotification(const ShellNotificationSentEventArgs& e) noexcept
	{
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		notify.uFlags |= NIF_INFO | NIF_SHOWTIP;
		StringCchCopyA(notify.szInfo, ARRAYSIZE(notify.szInfo), e.getMessage().c_str());
		StringCchCopyA(notify.szInfoTitle, ARRAYSIZE(notify.szInfoTitle), e.getTitle().c_str());
		notify.dwInfoFlags = NIIF_RESPECT_QUIET_TIME;
		if (e.getSeverity() == NotificationSeverity::Warning)
		{
			notify.dwInfoFlags |= NIIF_WARNING;
		}
		else if(e.getSeverity() == NotificationSeverity::Error)
		{
			notify.dwInfoFlags |= NIIF_ERROR;
		}
		else
		{
			notify.dwInfoFlags |= NIIF_USER;
		}
		Shell_NotifyIconA(NIM_MODIFY, &notify);
	}

	NOTIFYICONDATAA NotifyIcon::getBaseNotifyIconData() noexcept
	{
		NOTIFYICONDATAA notify;
		notify.cbSize = sizeof(notify);
		notify.hWnd = m_hwnd;
		notify.guidItem = m_guid;
		notify.uFlags = NIF_GUID;
		return notify;
	}
}
#endif
#ifdef _WIN32
#include "notifications/notifyicon.h"
#include <stdexcept>
#include <strsafe.h>
#include "aura.h"
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura::Notifications
{
	std::map<HWND, NotifyIcon*> NotifyIcon::m_icons = {};

	LRESULT NotifyIcon::notifyIconWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (m_icons.contains(hwnd))
		{
			return m_icons[hwnd]->handleMessage(uMsg, wParam, lParam);
		}
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}

	NotifyIcon::NotifyIcon(HWND hwnd, const NotifyIconMenu& contextMenu)
		: m_className{ StringHelpers::newGuid() },
		m_isHidden{ false },
		m_hwnd{ nullptr },
		m_contextMenu{ contextMenu }
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		//Create a window for the NotifyIcon
		WNDCLASSEXA windowClass;
		windowClass.cbSize = sizeof(windowClass);
		windowClass.style = CS_GLOBALCLASS;
		windowClass.lpfnWndProc = NotifyIcon::notifyIconWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = nullptr;
		windowClass.hIcon = (HICON)GetClassLongPtrA(hwnd, GCLP_HICON);
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);;
		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = m_className.c_str();
		windowClass.hIconSm = (HICON)GetClassLongPtrA(hwnd, GCLP_HICONSM);
		ATOM registeredClass{ RegisterClassExA(&windowClass) };
		if (registeredClass == 0)
		{
			throw std::runtime_error("Unable to register window class: " + std::to_string(GetLastError()) + ".");
		}
		m_hwnd = CreateWindowExA(0, m_className.c_str(), "AuraNotifyIcon", WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, nullptr, nullptr, nullptr);
		if (!m_hwnd)
		{
			throw std::runtime_error("Unable to create window: " + std::to_string(GetLastError()) + ".");
		}
		m_icons.insert({ m_hwnd, this });
		//Create the NotifyIcon
		if (CoCreateGuid(&m_guid) != S_OK)
		{
			throw std::runtime_error("Unable to create guid: " + std::to_string(GetLastError()) + ".");
		}
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		notify.uFlags |= NIF_ICON | NIF_TIP;
		notify.hIcon = (HICON)GetClassLongPtrA(hwnd, GCLP_HICON);
		StringCchCopyA(notify.szTip, ARRAYSIZE(notify.szTip), Aura::getActive().getAppInfo().getName().c_str());
		notify.uVersion = NOTIFYICON_VERSION_4;
		if (!Shell_NotifyIconA(NIM_ADD, &notify))
		{
			throw std::runtime_error("Unable to create NotifyIcon: " + std::to_string(GetLastError()) + ".");
		}
		Shell_NotifyIconA(NIM_SETVERSION, &notify);
		//Create context menu
		if (!m_contextMenu.empty())
		{

		}
	}

	NotifyIcon::~NotifyIcon() noexcept
	{
		//Destroy NotifyIcon
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		Shell_NotifyIconA(NIM_DELETE, &notify);
		//Destroy window for NotifyIcon
		m_icons.erase(m_hwnd);
		DestroyWindow(m_hwnd);
		UnregisterClassA(m_className.c_str(), nullptr);
	}

	void NotifyIcon::hide() noexcept
	{
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		notify.uFlags |= NIF_STATE;
		notify.dwState = NIS_HIDDEN;
		Shell_NotifyIconA(NIM_MODIFY, &notify);
		m_isHidden = true;
	}

	void NotifyIcon::show() noexcept
	{
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		notify.uFlags |= NIF_STATE;
		notify.dwState = 0;
		Shell_NotifyIconA(NIM_MODIFY, &notify);
		m_isHidden = false;
	}

	void NotifyIcon::showShellNotification(const ShellNotificationSentEventArgs& e) noexcept
	{
		NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
		notify.uFlags |= NIF_INFO;
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
		if (e.getAction() == "open" && std::filesystem::exists(e.getActionParam()))
		{
			m_openPath = e.getActionParam();
		}
		else
		{
			m_openPath = std::filesystem::path();
		}
		Shell_NotifyIconA(NIM_MODIFY, &notify);
	}

	NOTIFYICONDATAA NotifyIcon::getBaseNotifyIconData() noexcept
	{
		NOTIFYICONDATAA notify;
		notify.cbSize = sizeof(notify);
		notify.hWnd = m_hwnd;
		notify.guidItem = m_guid;
		notify.uFlags = NIF_MESSAGE | NIF_GUID | NIF_SHOWTIP;
		notify.uCallbackMessage = WM_NOTIFYICON_EVENT;
		return notify;
	}

	LRESULT NotifyIcon::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (uMsg == WM_NOTIFYICON_EVENT)
		{
			if (LOWORD(lParam) == NIN_SELECT)
			{

			}
			else if (LOWORD(lParam) == NIN_BALLOONTIMEOUT)
			{
				if (m_isHidden)
				{
					hide();
				}
			}
			else if (LOWORD(lParam) == NIN_BALLOONUSERCLICK)
			{
				if (!m_openPath.empty() && std::filesystem::exists(m_openPath))
				{
					ShellExecuteA(nullptr, "open", m_openPath.string().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
				}
			}
			else if (LOWORD(lParam) == WM_CONTEXTMENU)
			{

			}
			return 0;
		}
		return DefWindowProcA(m_hwnd, uMsg, wParam, lParam);
	}
}
#endif
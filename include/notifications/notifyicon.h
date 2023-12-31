#ifdef _WIN32
#ifndef NOTIFYICON_H
#define NOTIFYICON_H

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <shellapi.h>
#include "shellnotificationsenteventargs.h"

namespace Nickvision::Aura::Notifications
{
	/**
	 * @brief An icon for the system tray.
	 * @brief This API is only available on Windows.
	 */
	class NotifyIcon
	{	
	public:
		/**
		 * @brief Constructs a NotifyIcon. 
		 * @param hwnd The HWND handle of the main application window
		 * @throw std::logic_error Thrown if Aura::init() was not called yet 
		 */
		NotifyIcon(HWND hwnd);
		/**
		 * @brief Destructs a NotifyIcon. 
		 */
		~NotifyIcon() noexcept;
		/**
		 * @brief Shows a shell notification from the NotifyIcon.
		 * @param e ShellNotificationSentEventArgs
		 */
		void showShellNotification(const ShellNotificationSentEventArgs& e) noexcept;

	private:
		/**
		 * @brief Gets a basic NOTIFYICONDATAA struct for this NotifyIcon.
		 * @return NOTIFYICONDATAA
		 */
		NOTIFYICONDATAA getBaseNotifyIconData() noexcept;
		HWND m_hwnd;
		unsigned int m_id;
	};
}

#endif //NOTIFYICON_H
#endif